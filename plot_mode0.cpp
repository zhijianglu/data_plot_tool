#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "matplotlibcpp.h"
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace plt = matplotlibcpp;
using  namespace std;

std::vector<string> pre_colors  =  {"b","g","r","c","m","y","k","b","g","r","c","m","y","k"};
std::vector<string> pre_lines    =  {"-","-","-","-","-","-","-",":",":",":",":",":",":",":"};
std::vector<string> pre_points  =   {"^","1","2","s","p","*","h","+","x","D","d","v","^","o"};

int main(int argc, char **argv)
{

    if (argc == 1)
    {
        cout << "usage: "<<endl;
        cout << "        ./data_plot data1.txt data2.txt data3.txt plot_type win_size" << endl;
        cout << "data file formate: "<<endl;
        cout << "        first line of the data:             # [x-axis name] [y-axis name] [data name] plot_type \n"
             << "        second line of the data(optional):  * [line color] [mark type] \n"
                "plot_type: \n"
                "        1-----plot merg in one fig,"
                "        2-----plot respectively \n"<<endl;
        return -1;
    }

    //TODO load input param
    int file_num = argc-3;
    vector<string> file_path(file_num);
    cout<<"plot file:"<<endl;
    for (int i = 1; i < argc - 2; ++i)
    {
        file_path[i-1] = argv[i];
        cout<<"    "<<file_path[i-1]<<endl;
    }
    cout<<"plot type:"<<endl;

    int plot_type = atoi(argv[argc-2]);
    if(plot_type == 1)
        cout<<"    plot merg"<<endl;
    else if(plot_type == 2)
        cout<<"    plot respectively"<<endl;
    else{
        cout<<"    wrong input formate,please input plot type in lase param"<<endl;
        return -1;
    }

    int win_size = atoi(argv[argc-1]);


    //TODO 准备绘图信息变量
    vector<vector<double>> x_data(file_num);
    vector<vector<double>> x_inorder_data(file_num);
    vector<vector<double>> y_data(file_num);
    vector<vector<string>> data_label(file_num);

    //TODO 准备读取文件流
    vector<ifstream> file_in(file_num);

    double tmp;
    std::vector<double> x, max, mean, median, min, rmse, sse, std;


    std::string config_file_line;
    std::string cout_line;
    double axis_iter = 0;
    string s_data;
    for (int file_idx = 0; file_idx < file_num; ++file_idx)
    {
        file_in[file_idx].open(file_path[file_idx]);
        double data_cnt = 0;
        bool config_axis = false;
        bool config_linetype = false;
        if(file_in[file_idx].is_open())
            cout << "loading file:\n" << file_path[file_idx] << endl;
        else{
            cout << "can not open file:\n" << file_path[file_idx] << endl;
            return -1;
        }

        int win_cnt = 0;
        double _time_stemp = 0;
        double _data = 0;

        bool first = true;
        double init_time;

        while (std::getline(file_in[file_idx], config_file_line) && !file_in[file_idx].eof())
        {
            if (config_file_line.empty())
                continue;
            double last_time;
            std::istringstream iStream(config_file_line);
            iStream >> s_data;  //不管三七二十一先把第一个读进来

            if(!config_axis && s_data == "#"){    //读取配置轴名称行
                string x_axis,y_axis,data_name;
                iStream >> x_axis >> y_axis >> data_name;
                data_label[file_idx].push_back(x_axis); //0
                data_label[file_idx].push_back(y_axis); //1
                data_label[file_idx].push_back(data_name); //2
                config_axis = true;
                cout<<"        x_axis:"<<x_axis<<endl;
                cout<<"        y_axis:"<<y_axis<<endl;
                cout<<"        data_name:"<<data_name<<endl;
                continue;
            }

            if (!config_linetype && s_data == "*")  //读取配置曲线颜色和点的类型行
            {
                string line_type, color, point_type;
                iStream >> line_type >> color >> point_type;
                if (line_type == "" || color == "" || point_type == "")
                    continue;
                data_label[file_idx].push_back(line_type); //3
                data_label[file_idx].push_back(point_type); //4
                data_label[file_idx].push_back(color); //5
                cout << "        line_type:" << line_type << endl;
                cout << "        point_type:" << point_type << endl;
                cout << "        color:" << color << endl;
                config_linetype = true;
                continue;
            }

            if(first) {
                init_time = atof(s_data.c_str());
                first = false;
            }
            //写入数据
            _time_stemp += atof(s_data.c_str());
            double tmp_data;
            iStream >>tmp_data;
            _data += tmp_data;

            win_cnt++;
            if(win_cnt != win_size  ){
                cout<<"win_size:"<<win_size<<"  win_cnt:"<<win_cnt<<endl;
                continue;
            }
            cout<<"win_size:"<<win_size<<"  win_cnt:"<<win_cnt<<endl;
            x_data[file_idx].push_back(_time_stemp / win_size);// - init_time);//是否要设置第一帧时刻为0？
            y_data[file_idx].push_back(_data / win_size);
            cout << "mean data:" << to_string(_time_stemp / win_size) << "   " << _data / win_size << endl;
            x_inorder_data[file_idx].push_back(data_cnt);
            data_cnt++;

//TODO reset tmp data:
            win_cnt = 0;
            _time_stemp = 0;
            _data = 0;
//            cout << "frame time" << to_string(time_stemp) << endl;
        }

        if( win_cnt !=0 ){  //有余数
            x_data[file_idx].push_back(_time_stemp / win_cnt);// - init_time);//是否要设置第一帧时刻为0？
            y_data[file_idx].push_back(_data / win_cnt);
            cout << "mean data:" << to_string(_time_stemp / win_cnt) << "   " << _data / win_cnt << endl;
            x_inorder_data[file_idx].push_back(data_cnt);
        }

        file_in[file_idx].close();
        if(!config_linetype){   //如果文件中没有指明需要绘图的类型和颜色，装载默认的参数
            data_label[file_idx].push_back( pre_colors [file_idx]); //3
            data_label[file_idx].push_back( pre_lines  [file_idx]); //4
            data_label[file_idx].push_back( pre_points [file_idx]); //5
        }
    }

    //TODO 求平均值
    for (int file_idx = 0; file_idx < file_num; ++file_idx)
    {
        double sum = std::accumulate(std::begin(y_data[file_idx]), std::end(y_data[file_idx]), 0.0);
        double mean =  sum / y_data[file_idx].size(); //均值
        cout << "mean of "<< data_label[file_idx][2] <<" "<< data_label[file_idx][1] << " is " << mean << endl;
    }


    if(plot_type == 2){
        for (int file_idx = 0; file_idx < file_num; ++file_idx){
            plt::subplot(1, 1, 1);
            plt::named_plot(data_label[file_idx][2], x_data[file_idx], y_data[file_idx], data_label[file_idx][3]+data_label[file_idx][4]+data_label[file_idx][5]);
            plt::plot(x_data[file_idx], y_data[file_idx],data_label[file_idx][3]+data_label[file_idx][4]);
            plt::xlabel(data_label[file_idx][0]);
            plt::ylabel(data_label[file_idx][1]);
            plt::legend();
            plt::show();
        }
    }else if(plot_type == 1){
        plt::subplot(1, 1, 1);
        for (int file_idx = 0; file_idx < file_num; ++file_idx)
        {
            plt::named_plot(data_label[file_idx][2],
                            x_data[file_idx],
                            y_data[file_idx],
                            data_label[file_idx][3] + data_label[file_idx][4] + data_label[file_idx][5]);
            plt::plot(x_data[file_idx], y_data[file_idx], data_label[file_idx][3] + data_label[file_idx][4]);
            plt::xlabel(data_label[file_idx][0]);
            plt::ylabel(data_label[file_idx][1]);
            plt::legend();
        }
        plt::show();
    }
}
