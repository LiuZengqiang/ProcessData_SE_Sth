#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cfloat>

#define EPSILON 0.000001
using namespace std;

void process(float percent);

void average(string file_prefix, string file_suffix, int file_size, string out_file_path) {

    vector<vector<float>> result_data;
    float max_val = -FLT_MAX;
    float sum_val = 0.0f;

    for (int i = 0; i < file_size; i++) {
        if (i % 10 == 0) {
            process(static_cast<float>(i) / 100.0f);
        }
        string input_file_path = file_prefix + to_string(i) + file_suffix;
        ifstream input_file(input_file_path);

        if (!input_file.good()) {
            cout << "open \"" << input_file_path << "\" fail." << endl;
            return;
        }

        stringstream input_stream;
        input_stream << input_file.rdbuf();

        if (i == 0) {
            fstream out_file(out_file_path, fstream::out);
            string str;
            while (getline(input_stream, str)) {
                out_file << str;
                out_file << endl;
            }
            out_file.close();
        } else {
            fstream out_file(out_file_path, fstream::out | fstream::in);
            if (!out_file.good()) {
                cout << "open \"" << out_file_path << "\" fail." << endl;
                return;
            }
            stringstream output_stream;
            output_stream << out_file.rdbuf();
            out_file.close();
            out_file.open(out_file_path, fstream::out);
            if (!out_file.good()) {
                cout << "open \"" << out_file_path << "\" fail." << endl;
                return;
            }
            string str_in;
            string str_out;
            bool first_line = true;
            while (getline(input_stream, str_in) && getline(output_stream, str_out)) {
                if (first_line) {
                    out_file << str_in;
                    out_file << endl;
                    first_line = false;
                    continue;
                }
                float val = 0.0f;
                int index = str_in.length() - 1;
                while (index >= 0 && str_in[index] != ',') {
                    index--;
                }
                val = atof(str_in.substr(index + 1).c_str());
                index = str_out.length() - 1;
                while (index >= 0 && str_out[index] != ',') {
                    index--;
                }
                val += atof(str_out.substr(index + 1).c_str());

                if (i == file_size - 1) {
                    val /= file_size;
                    if (val < EPSILON) {
                        val = 0.0f;
                    }
                    max_val = max(max_val, val);
                    sum_val += val;
                }
                string str = str_out.substr(0, index + 1);
                out_file << str;
                out_file << val;
                out_file << endl;
            }
            out_file.close();
        }
    }
    cout << "result max:" << max_val << " sum:" << sum_val << endl;
}

int main() {
    vector<int> data_list = {1, 64, 128, 256, 384, 512, 576, 640, 704, 768, 832, 896, 960, 992, 1008, 1016, 1024, 1280,
                             1408, 1536, 1664, 2048};

    for (int i = 0; i < data_list.size(); ++i) {
        string prefix = "/home/sth/CLionProjects/ProcessData_SE_Sth/inputFile/" + to_string(data_list[i]) + "/0_";
        string suffix = "_receiver_after_smooth.csv";
        int size = 100;
        string out_file_path = "/home/sth/CLionProjects/ProcessData_SE_Sth/inputFile/average/" +
                               to_string(data_list[i]) + "_average.csv";
        cout << "Begin process density " << data_list[i] << " data." << endl;
        average(prefix, suffix, size, out_file_path);
        cout << "End process density " << data_list[i] << " data." << endl;
    }
    return 0;
}

void process(float percent) {
    int num = static_cast<int>(percent / 0.1f);
    cout << "[";
    for (int i = 0; i < num; i++) {
        cout << "-";
    }
    cout << ">";
    for (int i = num; i < 10; i++) {
        cout << " ";
    }
    cout << static_cast<int>(percent * 100) << "%" << endl;
}