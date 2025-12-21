/*Solution::Solution(const Instance& instance, const std::vector<int>& genome)
    : instance(instance), score_value(0) {
      pois_sequence.push_back(std::vector<int>());
      int indexDay = 0;
      int indexGene = 0;
      bool isAtHotel = false; // 1 if at hostel, 0 if at POI
      Point currentPlace = instance.getWorldMap().getStartingHostel();
      
      // initialize excluded hostels set
      std::set<int> excluded_hostels;
      std::set<int> excluded_pois;
      excluded_hostels.insert(instance.getWorldMap().getStartingHostelIndex());
      if (indexDay != instance.getDayCount() - 1) {
        excluded_hostels.insert(instance.getWorldMap().getEndingHostelIndex());
      }
      std::cout << "A\n\n";
      while (indexDay < instance.getDayCount() && indexGene < genome.size()) {
        Point nextPlace = instance.getWorldMap().getPOIByIndex(genome[indexGene]);
        float distance = instance.getWorldMap().getDistanceBetweenPoints(currentPlace, nextPlace);
        std::cout << "\n\n\nPOI sequence for day " << indexDay << ": ";
        for (int i = 0; i < pois_sequence[indexDay].size();i++) std::cout << pois_sequence[indexDay][i] << " ";
        std::cout << "\ncurrent Day: " << indexDay << "\n";
        std::cout << "B "<< genome[indexGene] << "\n";
        std::cout << "B1 " << instance.getDayByIndex(indexDay).getDuration() << std::endl;
        std::cout << "B2 " << getDayVisitsDuration(indexDay) << std::endl;
        std::cout << "B3 " << distance << std::endl;
        std::cout << "B4 " << (excluded_pois.find(genome[indexGene]) == excluded_pois.end()) << std::endl;

        if (instance.getDayByIndex(indexDay).getDuration() - getDayVisitsDuration(indexDay) > distance && 
            excluded_pois.find(genome[indexGene]) == excluded_pois.end()) {
          std::cout << "AJOUT POI " << genome[indexGene] << "\n";
          pois_sequence[indexDay].push_back(genome[indexGene]); 
          indexGene++;
          currentPlace = nextPlace;
          isAtHotel = false;
        }
        else {
          std::cout << "D\n";
          if (isAtHotel) {
            std::cout << "E\n";
            excluded_hostels.insert(intermediate_hostels.back());
            intermediate_hostels.pop_back();
            pois_sequence.pop_back();
            indexDay--;
            std::cout << "\n indexDay DECREASED: " << indexDay << "\n";
            isAtHotel = pois_sequence.back().empty();
            if (isAtHotel) {
              currentPlace = instance.getWorldMap().getHostelByIndex(
                  intermediate_hostels.back());
            } else {
              currentPlace = instance.getWorldMap().getPOIByIndex(
                  pois_sequence[indexDay].back());
            }
            int nextHostelId = findNearestHostel(currentPlace, excluded_hostels);
            if (nextHostelId == -1) {
              std::cout << "E1\n";
              continue;
            }
            else{
              
            }
          } 
          else {
            std::cout << "F\n";
            int nextHostelId;
            if (indexDay == instance.getDayCount() - 1) { //if last day
              std::cout << "G\n";
              if (getDayVisitsDuration(indexDay) + instance.getWorldMap().getDistanceBetweenPoints(
                currentPlace, instance.getWorldMap().getEndingHostel()) <= instance.getDayByIndex(indexDay).getDuration()) {
                  break;
              }
              nextHostelId = -1;
            }
            else{
              std::cout << "H\n";
              excluded_hostels.insert(intermediate_hostels.begin(), intermediate_hostels.end());
              if (indexDay == instance.getDayCount() - 1) {
                std::cout << "H1\n";
                excluded_hostels.erase(instance.getWorldMap().getEndingHostelIndex());
              }

              nextHostelId = findNearestHostel(instance.getWorldMap().getPOIByIndex(instance.genome[indexGene]), excluded_hostels);
            }
            if (nextHostelId == -1) {
              std::cout << "I " << nextHostelId << "\n";
              indexGene--;
              excluded_pois.insert(genome[indexGene]);
              pois_sequence[indexDay].pop_back();
              if (pois_sequence[indexDay].empty()) {
                isAtHotel = true;
                if (indexDay > 0) {
                  std::cout << "I1\n";
                  currentPlace = instance.getWorldMap().getHostelByIndex(
                      intermediate_hostels.back());
                } 
                else {
                  currentPlace = instance.getWorldMap().getStartingHostel();
                }
              } 
              else {
                currentPlace = instance.getWorldMap().getPOIByIndex(genome[indexGene]);
              }
              continue;
            }

            std::cout << "J " << nextHostelId << "\n";
            intermediate_hostels.push_back(nextHostelId);
            pois_sequence.push_back(std::vector<int>());

            // Move to next day
            indexDay++;
            currentPlace = instance.getWorldMap().getHostelByIndex(nextHostelId);
            isAtHotel = 1;
            excluded_pois.clear();
          }
        }
      }
    }*/