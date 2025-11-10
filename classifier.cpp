#include <iostream>
#include <fstream>
#include <string> 
#include <map>
#include <vector>
#include <tuple>
#include <cmath>
#include <set>
#include <sstream>
#include "csvstream.hpp"

using namespace std;





class Classifier {

    private:
   
        int total_post_count;
        int num_unique_words;
        set<string> vocabulary;
        map<string, int> posts_with_word_counts; //word -> count of posts with that word
        map<string, int> label_counts; //label -> count of posts with that label
        map<pair<string, string>, int> label_word_counts; // (label, word) -> posts with label containing word

    public:

        
        Classifier() : total_post_count(0), num_unique_words(0) {

            
        }
        
        void train(const string &trainin, bool train_only) {
            csvstream csvin(trainin);
            
            if (train_only) {cout << "training data:" << endl;}
            
            map<string, string> text_row;
            while (csvin >> text_row) {
                string label = text_row["tag"];
                string content = text_row["content"];


                if (train_only) {cout << "  label = " << label << ", content = " << content << endl;}


                total_post_count++;
                label_counts[label]++;
                
                set<string> post_words;
                string word;
                istringstream contentstream(content);
                while (contentstream >> word) {
                    //Adding to vocab
                    post_words.insert(word);
                }

                //count of posts with that word + add to vocab
                for (set<string>::iterator it = post_words.begin(); it != post_words.end(); it++) {
                    
                    posts_with_word_counts[*it]++;
                    vocabulary.insert(*it);
                    label_word_counts[{label, *it}]++;
                }
            }
        }
        
        void print_classifier_parameters() const {
          cout << "classifier parameters:" << endl;
          
          for (const pair<string, int>& label_and_count: label_counts) {
            string label = label_and_count.first;

            for (const string& word : vocabulary) {
              
              double log_likelihood;
              int count;

              //log likelihood of word given label c
              count = label_word_counts.at({label, word});
              log_likelihood = log(static_cast<double>(count) / label_counts.at(label));
              
              //log likelhood - not present

              //other log likelihoods


            }

          }
          


        }



        void print_classes_and_num_examples() const {
            cout << "classes";
            

            for (const pair<const string, int> label_and_count : label_counts) {
                string label = label_and_count.first;
                int label_count = label_and_count.second;
                double double_label_count = static_cast<double>(label_count);

                double log_prior = log(double_label_count / total_post_count);

                //LOG PRIOR
                cout << label << ", " << label_count << " examples, log-prior = " << log_prior << endl;

                cout << endl;
            }
        }


        int get_total_post_count() const {
            return total_post_count;
        }

        void set_total_post_count(int count) {
            total_post_count = count;
        }

        int get_num_unique_words() const {
            return num_unique_words;
        }




};



int main(int argc, char *argv[]) {



  cout.precision(3);
  if (argc != 3 && argc != 2) {
    cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
    return 1;
  }
   
  bool train_only = (argc == 2); 

  ifstream trainin(argv[1]);
  if (!trainin.is_open()) {
      cout << "Error opening file: " << string(argv[1]) << endl;
  return 1;
  }

  Classifier classifier;
  classifier.train(argv[1], train_only);

  cout << "trained on " << classifier.get_total_post_count() << " examples" << endl;

  if (train_only) {
    cout << "vocabulary size = " << classifier.get_num_unique_words() << endl;
  }

  cout << endl;

  if (train_only) {
    classifier.print_classes_and_num_examples();
    classifier.print_classifier_parameters();
    cout << endl;
  }

  if (argc == 3) {

    ifstream testin(argv[2]);
    if (!testin.is_open()) {
      cout << "Error opening file: " << string(argv[2]) << endl;
      return 1;
    }

    //Test only files output

  }



}