#include <iostream> 
#include <fstream>
#include <set>
#include "csvstream.hpp"
#include <map>
#include <cmath> 

using namespace std; 

// EFFECTS: Return a set of unique whitespace delimited words.x
set<string> unique_words(const string &str) {
  istringstream source(str);
  set<string> words;
  string word;
  while (source >> word) {
    words.insert(word);
  }
  return words;
}

//classifer class 
class classifier {
    //each label C and word W, the number of posts with label C that contain W
    map<pair<string, string>, int> myMap; 
    //finding the value in a key 
    //my_map[{"euchre", "bower"}]
    int numPosts; 
    int uniqueWords; //for keeping track of all the unique words; 
    string training; 
    string testing; 
    map<string, int > wordMap; 
    map<string, int> labelMap; 
    public : 

        // constructor
        classifier() 
        : numPosts(0), uniqueWords(0),
        training(""), testing("") {} 

        // constructor2 

        classifier(string trainFile, string testFile) 
        : numPosts(0), uniqueWords(0), training(trainFile),testing(testFile) {} 


        double logPrior(string label){ 
            double result; 
            double TrainingWithLabel;
            TrainingWithLabel = labelMap[label]; 
            result = log((TrainingWithLabel/numPosts));
            return result; 
        }

        double logLikelihood(string label, string word){
            double result; 
            double TrainingWithLabelWithWord;
            double NumTrainingPostWithLabel; 
            TrainingWithLabelWithWord = myMap[{label,word}]; 
            NumTrainingPostWithLabel = labelMap[label]; 

            if((TrainingWithLabelWithWord == 0) && (wordMap.find(word) != wordMap.end())){
                result = log(((double)wordMap[word]/(double)numPosts));
                return result; 
            }
            else if (wordMap.find(word) == wordMap.end()){
                result = log((1.0/(double)numPosts));
                return result; 
            }
            
            else {
                result = log(TrainingWithLabelWithWord/NumTrainingPostWithLabel); 
                return result; 
            }
        }

        pair<string, double> Prediction_logProbability(set<string> UniqueWords){
            pair<string, double> winner; 
            set<string>::iterator itr;
            string label; 
            string word; 
            double temp = 0; 
            int counter = 0; 
            for (const auto& entry : labelMap) {
                counter++; 
                double total = 0; 
                label = entry.first; 
                total += logPrior(label); 
                for (itr = UniqueWords.begin(); 
                    itr != UniqueWords.end(); itr++) 
                {
                    string word = *itr; 
                    total += logLikelihood(label, word);
                }
                if(counter == 1){
                    temp = total; 
                    winner = make_pair(label, temp); 
                }
                else if(total > temp){
                    temp = total; 
                    winner = make_pair(label, temp); 
                }
            }
            return winner; 
        }

        void addUniqueWords(set<string> unique){
            set<string>::iterator itr;
            // Displaying set elements
            for (itr = unique.begin(); 
                itr != unique.end(); itr++) 
            {
                string word = *itr; 
                if(wordMap.find(word) == wordMap.end()){
                    wordMap.insert({word,0});
                    wordMap[word]++;
                    uniqueWords++; 
                }
                else{
                    wordMap[word]++; 
                }
               
            }
        }

        void addLabel(string label){
            if(labelMap.find(label) == labelMap.end()){
                    labelMap.insert({label,0});
                    labelMap[label]++;
                }
                else{
                    labelMap[label]++; 
                }
        }

        void addMyMap(string label, set<string> context){
            set<string>::iterator itr;
            // Displaying set elements
            for (itr = context.begin(); 
                itr != context.end(); itr++) 
            {
                string word = *itr; 
                pair<string,string> key = {label,word}; 

                if(myMap.find(key) == myMap.end()){
                    myMap.insert({key, 0});
                    myMap[key]++;
                }
                else{
                    myMap[key]++; 
                }
               
            }
        }

        void train(){
            csvstream csvin(training); 
            map<string,string> row; 
            set<string> setofUnique; 
            string label; 
            string context; 

            while(csvin >> row){
                for (auto &col:row){
                    string column_name = col.first; 
                    string datum = col.second; 

                    if (column_name == "content"){
                        setofUnique = unique_words(datum); 
                        addUniqueWords(setofUnique); 
                    }
                    if (column_name == "tag"){
                        label = datum; 
                        addLabel(label); 
                    }
                }
                addMyMap(label, setofUnique); 
                numPosts++; 
            }
        }

        void testingDebug(){
            string label; 
            string word;
            cout << "\nclasses:" << endl; 
            for (auto it = labelMap.begin(); it != labelMap.end(); ++it) {

                label = it->first; 
                cout << "  " << it->first << ", " << labelMap[label] 
                << " examples, log-prior = " 
                << logPrior(label) << endl; 
                
            }
            cout << "classifier parameters:" << endl; 
            for (auto it = myMap.begin(); it != myMap.end(); ++it) {

                label = it->first.first; 
                word = it->first.second; 
                cout << "  " << label << ":" << word << ", count = " 
                << myMap[{label, word}]  
                << ", log-likelihood = " << logLikelihood(label, word) 
                << endl; 
        
            }
        }

        void printingDebugData(){
                csvstream csvin(training); 
                map<string,string> row; 
                string label; 
                string context; 

                cout << "training data:" << endl; 
                while(csvin >> row){
                    for (auto &col:row){
                        string column_name = col.first; 
                        string datum = col.second; 

                        if (column_name == "content"){
                            context = datum; 
                        }
                        if (column_name == "tag"){
                            label = datum; 
                        }
                    }
                    cout << "  label = " << label << ", content = " 
                    << context << endl;
                }
                cout << "trained on " << numPosts << " examples" << endl; 
                cout << "vocabulary size = " << uniqueWords << endl; 
            }

        void testingData(bool debug){
            if(!debug){
            cout << "trained on " << numPosts << " examples" << endl; 
            }
            cout << "\ntest data:" << endl; 
            csvstream csvin(testing); 
            map<string,string> row; 
            set<string> setofUnique; 
            string label; 
            string context; 
            int CorrectCounter = 0; 
            int Counter = 0; 
            while(csvin >> row){
                for (auto &col:row){
                    string column_name = col.first; 
                    string datum = col.second; 
                    if (column_name == "content"){
                        context = datum; 
                        setofUnique = unique_words(datum); 
                    }
                    if (column_name == "tag"){
                        label = datum; 
                    }
                }
                pair<string, double> winner; 
                winner = Prediction_logProbability(setofUnique); 
                string predicted = winner.first; 
                double logProbability = winner.second; 
                cout << "  correct = " << label << ", predicted = " << predicted 
                << ", log-probability score = " << logProbability << endl; 
                cout << "  content = " << context << "\n" << endl ;

                if(label == predicted)CorrectCounter++; 
                Counter++; 
            }
            cout << "performance: " << CorrectCounter << " / " << Counter << 
            " posts predicted correctly" << endl << endl;
        }
};


bool check_errors(int argc, char* argv[]){
    string file = argv[1]; 
    string file2 = argv[2]; 
    if(argc == 4){
        string debug = argv[3]; 
        if (argv[3] != debug && argc == 4){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
        << endl;
        return true; 
    }   
    }
    ifstream trainFile; 
    ifstream testFile; 
    trainFile.open(file);
    testFile.open(file2); 
    if (argc < 3 || argc > 5){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" 
        << endl;
        return true; 
    }
    if (!trainFile.is_open()){
        cout << "Error opening file: " << file << endl;
        return 1;
    }
    else if (!testFile.is_open()){
        cout << "Error opening file: " << file2 << endl;
        return 1;
    }
    else return false; 
}

int main(int argc, char* argv[]){
    cout.precision(3); 
    bool debug = false;
    string IsDebug = "--debug";  
    if (check_errors(argc, argv)){
        return 1; 
    }
    if(argc == 4 && argv[3] == IsDebug)debug = true; 

    string trainFile = argv[1]; 
    string testFile = argv[2]; 
   
    classifier predictor(trainFile, testFile); 
    
    if(debug){
        predictor.train(); 
        predictor.printingDebugData(); 
        predictor.testingDebug(); 
        predictor.testingData(debug); 
    }
    else {
        predictor.train(); 
        predictor.testingData(debug); 
    }

}