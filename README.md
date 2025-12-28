# OptiChemin

The problem is a variant of the **Multi-Trip Orienteering Problem with Time Windows (MTOPTW)**.

This problem is a famously hard (NP-Hard) combinatorial optimization problem. To solve it within a couple of minutes, we cannot rely on brute force and we must use a Genetic Algorithm.

However, this problem is constraint-rich : a "naive" GA would quickly fail because the numerous rules (time windows, days, hostel links) make 99% of random solutions invalid.

We can solve this by separating the problem into two layers :

- The **Genotype** (what the algorithm evolves)
- The **Phenotype** (the actual schedule)

## 1. Representation : The "Giant Tour" (Genotype)

The most critical design decision in a GA is how you represent a solution.

### The Decision : Permutation Encoding

We represent an individual not as a complex schedule of days and times, but simply as a **single list** (_permutation_) of all available POIs, often called a "Giant Tour".

- Example : `[POI_5, POI_1, POI_9, POI_2, etc.]`

### The Alternative : Direct Representation

We could have tried to create a class with `vector<vector<int>>` representing each day directly in the chromosome.

#### Why we dismissed it

- **Crossover Nightmare** : If Parent A has `POI_5` on Day 1, and Parent B has `POI_5` on Day 3, combining them often leads to `POI_5` appearing twice (invalid) or disappearing entirely.
- **Constraint Hell** : Randomly moving a POI from Day 1 to Day 2 might violate the time window or day duration, resulting in an invalid child that yields 0 points. We would spend all of our computing power fixing broken solutions.

#### Why permutation wins

A permutation is an always-valid list of **priorities**. The "Giant Tour" says : "I want to visit `POI_5` first. If it fits, great. Then I want to visit `POI_1`...".
It shifts the burden of validity to the **decoder**.

## 2. The Decoder : Converting Genes to Schedules

The decoder is the translation layer. It takes the "Giant Tour" and cuts it into valid days.
This is where your constraints (Opening Times, Day Durations) are enforced.

### The Algorithm : Greedy Split with Lookahead

We iterate through the Giant Tour list and fill **Day 1** as much as possible.
When the next POI in the list doesn't fit (because the day is full or the POI closes too soon), we "close" Day 1 and start Day 2.

TODO : What if it only took one day and moved on ?

### The "Smart Hostel" Heuristic (Critical)

The problem requires choosing intermediate hostels.

#### The Alternative (Dismissed)

Including Hostel IDs in the chromosome (e.g., `[POI_1, POI_2, HOSTEL_3, POI_4...]`).
It increases the search space massively.
The algorithm would waste time trying `Hostel_1` when `Hostel_2` is obviously better geographically.

#### The Solution : Deterministic Calculation

- Don't let the algorithm guess the hostel : **calculate it**.
- When the decoder decides to end Day `i` at `POI_A` and start Day `i+1` at `POI_B`, it iterates through all available hostels to find the one that minimizes the total travel time (`POI_A -> Hostel -> POI_B`)
- TODO : what if no hostel fits ?
- This removes thousands of "bad variables" from the optimization process, making the GA converge much faster

## 3. Genetic Operators

Since our chromosome is a permutation, we cannot use standard single-point crossover (which works for binary strings). We need operators that respect order and uniqueness.

### Crossover : Order Crossover (OX1)

- **Goal** : Preserve sub-tours. If a parent has found a great sequence that fits perfectly together, we want to pass that chunk to the child intact.
- **How it works** : We copy a random slice from Parent A to the child. Then, we fill the empty spots with the missing POIs in the order they appear in Parent B.
- **Why not PMX ?** : Partially Mapped Crossover preserves absolute positions better, but in routing problems, _relative_ order (who comes after whom) is more important than absolute position (whether `POI_A` is the 5th or 6th visited).

### Mutation : Swap vs. 2-Opt (Inversion)

- **Swap** : Pick two random POIs and swap them. This is good for "teleporting" a POI to a different day.

- **2-Opt (Inversion)** : Pick two indices and _reverse_ the sequence between them. This is vital because in Euclidean 2D maps, the shortest path never crosses itself. If your tour has a "knot" (crossing lines), a simple reversal (2-Opt) untangles it and reduces travel time instantly. This is the single most powerful mutation for routing problems.

TODO : What if the inversion makes the path impossible because of opening hours ?

## 4. Fitness and Constraints

Our fitness function is simply the total score collected. We do not substract points for invalid solutions, because the decoder never builds any.
If a POI causes a time violation, it is simply skipped (not added to the schedule).
The solution remains valid, but gets a lower score because it visited fewer POIs. This "natural penalty" is much more robust.

## 5. Summary of the architecture

1. **Solver** : Manages the population and evolution
2. **Genotype** : `vector<int> giant_tour` (the priority list).
3. **Phenotype** : `Solution` object (The actual valid schedule).
4. **Eval** : `Genotype` -> Decoder -> `Phenotype` -> Score -> `Fitness`
5. **Decoder Logic** :

- Fill Day N with POIs from the list.
- If full : Pich the best intermediate hostel and start day N+1

```cpp
// temp/src/solver.cpp

#include "solver.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <chrono>

Solver::Solver(const Instance& instance, int seed) : inst(instance), rng(seed) {}

// --- THE SMART HOSTEL SELECTOR ---
// Theory: Instead of guessing the hostel, we calculate the optimal one.
// We look at the Last POI of Day N and the First POI of Day N+1.
// We pick the hostel that minimizes the specific detour required to bridge them.
int Solver::selectBestHostel(int last_poi_id, int next_poi_id, float current_day_time, float max_day_duration) const {
    int best_hostel = -1;
    float best_added_dist = std::numeric_limits<float>::max();

    // Iterate over ALL hostels to find the best bridge
    for (int h = 0; h < inst.getHostelCount(); ++h) {
        // Calculate distance from the day's last POI to this specific hostel
        float dist_to_hostel = inst.getDistanceHostelPOI(h, last_poi_id); // Symmetric
        
        // CONSTRAINT CHECK: Can we even reach this hostel before the day ends?
        if (current_day_time + dist_to_hostel <= max_day_duration) {
            
            // Heuristic Cost: Minimize (Distance to Hostel) + (Distance from Hostel to NEXT POI)
            // If there is no next POI (end of tour), we just minimize distance to hostel.
            float dist_from_hostel = (next_poi_id >= 0) ? inst.getDistanceHostelPOI(h, next_poi_id) : 0.0f;
            
            float total_cost = dist_to_hostel + dist_from_hostel;

            if (total_cost < best_added_dist) {
                best_added_dist = total_cost;
                best_hostel = h;
            }
        }
    }
    return best_hostel;
}

// --- THE DECODER (Genotype -> Phenotype) ---
// Theory: This is the "Greedy Split" algorithm. 
// It converts the permutation (Giant Tour) into a valid schedule.
Solution Solver::decode(const std::vector<int>& giant_tour) const {
    Solution sol;
    sol.score_value = 0;
    sol.pois_sequence.resize(inst.getDayCount());
    sol.start_dates.resize(inst.getDayCount(), 0.0f);

    int tour_idx = 0;
    
    // Day 0 always starts at the designated starting hostel.
    int current_start_hostel = inst.getStartingHostelID();

    for (int day = 0; day < inst.getDayCount(); ++day) {
        float current_time = 0.0f; // Relative to day start (0.0)
        float max_duration = inst.getDayDuration(day);
        
        // Position tracking: -1 indicates we are at a Hostel, >=0 is a POI
        int current_poi_id = -1; 
        int current_hostel_id = current_start_hostel; 

        // 1. Try to fill the current day with POIs from the Giant Tour
        while (tour_idx < giant_tour.size()) {
            int candidate_poi = giant_tour[tour_idx];
            
            // A. Calculate travel time from current location
            float dist = 0.0f;
            if (current_poi_id == -1) {
                // Coming from a hostel
                dist = inst.getDistanceHostelPOI(current_hostel_id, candidate_poi);
            } else {
                // Coming from another POI
                dist = inst.getDistancePOIPOI(current_poi_id, candidate_poi);
            }

            float arrival = current_time + dist;
            float opening = inst.getPOIOpeningTime(candidate_poi);
            float closing = inst.getPOIClosingTime(candidate_poi);

            // B. Handle Time Windows (Wait if early)
            if (arrival < opening) arrival = opening;

            // C. Check Validity
            bool possible = true;
            
            // Rule 1: Must arrive before closing
            if (arrival > closing) possible = false;

            // Rule 2: Must have enough time to reach *a* hostel afterwards.
            // (We don't know WHICH hostel yet, but if we can't reach the CLOSEST one, we are dead).
            if (possible) {
                if (day == inst.getDayCount() - 1) {
                    // Last Day: Must reach the strict Ending Hostel
                    float dist_end = inst.getDistanceHostelPOI(inst.getEndingHostelID(), candidate_poi);
                    if (arrival + dist_end > max_duration) possible = false;
                } else {
                    // Normal Day: Must reach ANY hostel.
                    // Optimization: Quick check against the nearest hostel or just iterate all.
                    bool can_reach_any = false;
                    for(int h=0; h<inst.getHostelCount(); ++h) {
                         // Check if we can reach hostel 'h' before the day ends
                         if (arrival + inst.getDistanceHostelPOI(h, candidate_poi) <= max_duration) {
                             can_reach_any = true; 
                             break;
                         }
                    }
                    if (!can_reach_any) possible = false;
                }
            }

            if (possible) {
                // D. Commit the Move
                sol.pois_sequence[day].push_back(candidate_poi);
                sol.score_value += (int)inst.getPOIScore(candidate_poi);
                current_time = arrival;
                current_poi_id = candidate_poi;
                current_hostel_id = -1; // We are now at a POI, not a hostel
                tour_idx++; // Move to next gene
            } else {
                // E. Stop the Day
                // If this POI doesn't fit, we stop filling this day. 
                // We do NOT increment tour_idx, so this POI will be considered for the NEXT day.
                break;
            }
        }

        // 2. End of Day: Choose Intermediate Hostel
        // We need to pick a hostel to sleep at, unless it's the very last day (fixed end).
        if (day < inst.getDayCount() - 1) {
            
            // Look ahead: What is the first POI of the NEXT day?
            // This helps us pick a hostel that leads towards that target.
            int next_poi_target = (tour_idx < giant_tour.size()) ? giant_tour[tour_idx] : -1;
            
            int last_poi_visited = (sol.pois_sequence[day].empty()) ? -1 : sol.pois_sequence[day].back();
            
            int chosen_hostel = -1;

            if (last_poi_visited == -1) {
                // Edge case: We visited NO POIs today.
                // We stay at the same hostel or move to a "better" one (essentially a wasted day of travel).
                // Default to 0 or current to avoid crashes.
                 chosen_hostel = current_start_hostel; 
            } else {
                // Use the smart selector
                chosen_hostel = selectBestHostel(last_poi_visited, next_poi_target, current_time, max_duration);
            }
            
            // Fail-safe if no hostel is reachable (shouldn't happen due to logic above, but safe to handle)
            if (chosen_hostel == -1) chosen_hostel = 0; 

            sol.intermediate_hostels.push_back(chosen_hostel);
            current_start_hostel = chosen_hostel; // This becomes the start for the next day
        }
    }
    return sol;
}

// --- GENETIC OPERATORS ---

Individual Solver::crossover(const Individual& p1, const Individual& p2) {
    // Operator: Order Crossover (OX1)
    // Theory: Preserves a subsequence of relative orders from P1, fills gaps from P2.
    // Excellent for "Giant Tour" representations.
    
    Individual child;
    child.giant_tour.resize(p1.giant_tour.size());
    
    int size = (int)p1.giant_tour.size();
    int start = std::uniform_int_distribution<int>(0, size - 2)(rng);
    int end = std::uniform_int_distribution<int>(start + 1, size - 1)(rng);
    
    // 1. Copy segment from Parent 1
    std::vector<bool> taken(inst.getPOICount(), false);
    for(int i=start; i<=end; ++i) {
        child.giant_tour[i] = p1.giant_tour[i];
        taken[p1.giant_tour[i]] = true;
    }
    
    // 2. Fill remaining slots with genes from Parent 2
    // We scan P2 and take any gene that isn't already in the child.
    int current_p2_idx = 0;
    for(int i=0; i<size; ++i) {
        if (i >= start && i <= end) continue; // Skip the P1 segment we already filled
        
        // Find next available gene in P2
        while(taken[p2.giant_tour[current_p2_idx]]) {
            current_p2_idx++;
        }
        child.giant_tour[i] = p2.giant_tour[current_p2_idx];
        taken[p2.giant_tour[current_p2_idx]] = true;
    }
    
    return child;
}

void Solver::mutate(Individual& ind) {
    // Operator: 2-Opt (Inversion) & Swap
    int size = (int)ind.giant_tour.size();
    if (size < 2) return;

    std::uniform_int_distribution<int> dist(0, size - 1);
    
    // Randomly choose mutation type
    // 2-Opt is usually more powerful for routing, so we might give it higher weight.
    int type = dist(rng) % 2; 
    
    int i = dist(rng);
    int j = dist(rng);
    while (i == j) j = dist(rng);
    
    if (type == 0) {
        // SWAP: Good for moving a POI to a completely different time
        std::swap(ind.giant_tour[i], ind.giant_tour[j]);
    } else {
        // INVERSION (2-OPT): Good for untangling crossing paths
        if (i > j) std::swap(i, j);
        std::reverse(ind.giant_tour.begin() + i, ind.giant_tour.begin() + j + 1);
    }
}

// --- MAIN LOOP ---

Solution Solver::solve(int time_limit_seconds) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 1. Initialization
    std::vector<Individual> population(population_size);
    std::vector<int> base_tour(inst.getPOICount());
    for(int i=0; i<inst.getPOICount(); ++i) base_tour[i] = i;

    // Create random population
    for(auto& ind : population) {
        ind.giant_tour = base_tour;
        std::shuffle(ind.giant_tour.begin(), ind.giant_tour.end(), rng);
        Solution s = decode(ind.giant_tour);
        ind.fitness = s.score_value;
    }
    
    int generation = 0;
    
    // 2. Evolution Loop
    while(true) {
        // Time Check
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() >= time_limit_seconds) {
            break;
        }

        // Sort by fitness (descending)
        std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b){
            return a.fitness > b.fitness;
        });

        std::vector<Individual> next_gen;
        
        // Elitism: Keep the best few solutions unchanged
        int elites = (int)(population_size * elitism_rate);
        for(int i=0; i<elites; ++i) next_gen.push_back(population[i]);

        // Generate the rest
        while(next_gen.size() < population_size) {
            // Tournament Selection: Pick K random, choose best.
            // Helps maintain diversity vs simply picking the global top.
            int best_idx = -1;
            int best_fit = -1;
            for(int k=0; k<tournament_size; ++k) {
                int idx = std::uniform_int_distribution<int>(0, population_size/2)(rng); 
                if (population[idx].fitness > best_fit) {
                    best_fit = population[idx].fitness;
                    best_idx = idx;
                }
            }
            Individual& p1 = population[best_idx];
            
            // Random second parent (simple and fast)
            Individual& p2 = population[std::uniform_int_distribution<int>(0, population_size-1)(rng)];

            // Crossover
            Individual child = crossover(p1, p2);
            
            // Mutation
            if (std::uniform_real_distribution<float>(0,1)(rng) < mutation_rate) {
                mutate(child);
            }
            
            // Evaluate
            Solution s = decode(child.giant_tour);
            child.fitness = s.score_value;
            next_gen.push_back(child);
        }
        population = next_gen;
        generation++;
        
        if (generation % 100 == 0) { // Reduced logging frequency for speed
            std::cout << "Gen " << generation << " Best: " << population[0].fitness << std::endl;
        }
    }

    // Return best found
    std::sort(population.begin(), population.end(), [](const Individual& a, const Individual& b){
        return a.fitness > b.fitness;
    });
    return decode(population[0].giant_tour);
}
```

## 6. Corrections and improvements

### The risk of closing the day too early

In the "Basic Greedy Split" described earlier (if next doesn't fit, then break and jump to the next day), we risk having a day with 3 hours left because the very next POI in the list happened to be far away or closed. This is a massive waste of potential score.

The solution is "Gap Filling" (Look-ahead) : Instead of strictly breaking the loop when the `candidate_poi` does not fit, a smarter decoder continues to scan the rest of the `giant_tour` to find smaller/closer POIs that _do_ fit in the remaining time.

It can be implemented by replacing the `break` in the `decode` loop by a `continue`, effectively skipping the problematic POI _for now_.

- _Current Logic_ : "Next POI is impossible ? Okay, day over."
- _Better Logic_ : "Next POI is impossible ? Skip it. Check the one after. Does it fit ? Yes ! Add it".

This change breaks the strict "sequence" of the chromosome, which can sometimes confuse the crossover operator (order crossover assumes strict adherence to sequence). However for this specific problem, gap filling might be often superior because filling the time bucket is the #1 priority.

It might be worth comparing both options.

### The "Stranded at POI" Problem

#### The Scenario

You are at `POI_X`, you have 1 hour left in the day. You decide to visit `POI_Y` which takes 40 minutes. You arrive with 20 minutes left.
However, the closest hostel to `POI_Y` is 30 minutes away : you are stranded.

#### The Solution : the "Safe Return" Constraint

We can solve this by checking the **return trip** before we even decide to go to `POI_Y`.

In the `decode` function, the validity check for `candidate_poi` is not just `Arrival <= ClosingTime`, but rather `Arrival + Distance(Candidate, NearestHostel) <= DayDuration`

If this condition is false, we do not go to `candidate_poi`. We treat it as "doesn't fit" and skip it.

### Code Changes

```cpp
// Inside Solves::decode(...)
// ... inside the day loop ...

// We iterate through the ENTIRE remaining giant tour to find ANY POI that fits
// (Gap Filling Strategy)
for (int i = tour_idx; i < giant_tour.size(); ++i) {
    int candidate_poi = giant_tour[i];
    
    // Check if we already visited this POI in a previous gap-fill on this same day
    // (You need a small 'visited' flag array or check existence)
    if (is_already_visited[candidate_poi]) continue;

    // 1. Calculate Arrival
    float dist = (current_poi_id == -1) 
                 ? inst.getDistanceHostelPOI(current_hostel_id, candidate_poi)
                 : inst.getDistancePOIPOI(current_poi_id, candidate_poi);
    
    float arrival = std::max(current_time + dist, inst.getPOIOpeningTime(candidate_poi));

    // 2. Strict Hard Constraints
    bool fits = true;
    if (arrival > inst.getPOIClosingTime(candidate_poi)) fits = false;

    // 3. The "Safe Return" Constraint (Question 2)
    if (fits) {
        bool can_return_to_any_hostel = false;
        // Optimization: Pre-calculate the 'nearest hostel distance' for every POI 
        // to avoid this inner loop.
        for (int h = 0; h < inst.getHostelCount(); ++h) {
            if (arrival + inst.getDistanceHostelPOI(h, candidate_poi) <= max_duration) {
                can_return_to_any_hostel = true;
                break;
            }
        }
        if (!can_return_to_any_hostel) fits = false;
    }

    if (fits) {
        // Add to solution
        sol.pois_sequence[day].push_back(candidate_poi);
        sol.score_value += (int)inst.getPOIScore(candidate_poi);
        is_already_visited[candidate_poi] = true;
        
        current_time = arrival;
        current_poi_id = candidate_poi;
        current_hostel_id = -1;
        
        // IMPORTANT: Do NOT break. Keep scanning 'i' to find MORE fillers 
        // if time permits!
    }
}
// After scanning the whole list, the day is truly full.
```
