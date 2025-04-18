#include "Questions.hpp"


using json = nlohmann::json;

// load data from json files
void Question::loadData() {
    fstream file;
    vector<string> data = {
        "data/easy.json",
        "data/medium.json",
        "data/hard.json"
    };
    for (const auto& filename : data) {
        srand(time(0));
        file.open(filename, ios::in);
        if (!file) {
            cout << "File not found: " << filename << endl;
            return;
        }
        json Doc{json::parse(file)};

        // get results
        set<int> existIndexes;
        json data = Doc["results"];
        

        int n = 0;
        while (existIndexes.size() < 5) {
            int random_index = rand() % Doc["results"].size();

            if (existIndexes.count(random_index) == 0) {
                existIndexes.insert(random_index);
        
                this->questionText = data[random_index]["question"];
        
                // Prepare answers
                vector<string> tempOptions(4);
                tempOptions[0] = data[random_index]["correct_answer"];
                tempOptions[1] = data[random_index]["incorrect_answers"][0];
                tempOptions[2] = data[random_index]["incorrect_answers"][1];
                tempOptions[3] = data[random_index]["incorrect_answers"][2];
        
                // Randomly shuffle
                random_shuffle(tempOptions.begin(), tempOptions.end());
        
                // Find where the correct answer ended up
                for (int i = 0; i < 4; ++i) {
                    this->options[i] = tempOptions[i];
                    if (tempOptions[i] == data[random_index]["correct_answer"]) {
                        this->correctOptionIndex = i;
                    }
                }
            }
        }
        file.close();
    }
}