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
//    std::vector<double> x, max, mean, median, min, rmse, sse, std;


    std::string config_file_line;
    std::string cout_line;
    double axis_iter = 0;
    string s_data;

    double hz = 20.0;
    double freq_ctrl = win_size*(1.0/hz);
    std::vector<double> init_time(file_num);
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

        double _time_stemp = 0;
        double _data = 0;

        bool first = true;

        double last_time = 0;
        double intergrate_time = 0;

        while (std::getline(file_in[file_idx], config_file_line) && !file_in[file_idx].eof())
        {
            if (config_file_line.empty())
                continue;
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

            //窗内加入数据
            _time_stemp = atof(s_data.c_str());
            double tmp_data;
            iStream >>tmp_data;
            _data += tmp_data;


            if(first){
                init_time[file_idx] = _time_stemp;  //记录第一时刻
                first = false;
                x_data[file_idx].push_back(_time_stemp);
                y_data[file_idx].push_back(_data);
            }

            intergrate_time = _time_stemp-x_data[file_idx].back();

            if ((win_size !=0 )&& abs(intergrate_time-freq_ctrl) > ( (1.0 / (4.0*hz)) ) ) //默认总能积分到的
            {
                //小于周期的一半，认定为同一帧,否者不允许
                cout<<"intergrate_time:"<<intergrate_time<<"  freq_ctrl:"<<freq_ctrl<<"  theshold:"<<( (1.0 / (4.0*hz)) )<<endl;

            }else{
                cout<<"===============intergrate_time:"<<intergrate_time<<"  freq_ctrl:"<<freq_ctrl<<"  theshold:"<<( (1.0 / (4.0*hz)) )<<endl;
                //小于周期的一半，认定为同一帧,否者不允许

                x_data[file_idx].push_back(_time_stemp);//是否要设置第一帧时刻为0？
                y_data[file_idx].push_back(_data);

//      TODO: 复位所有值

                _data = 0;
                data_cnt++;
            }

        }

        file_in[file_idx].close();
        if(!config_linetype){   //如果文件中没有指明需要绘图的类型和颜色，装载默认的参数
            data_label[file_idx].push_back( pre_colors [file_idx]); //3
            data_label[file_idx].push_back( pre_lines  [file_idx]); //4
            data_label[file_idx].push_back( pre_points [file_idx]); //5
        }
    }

    auto smallest = std::min_element(std::begin(init_time), std::end(init_time));
    //TODO 求平均值  横轴平移
    string out_fig_name = data_label[1][1].substr(0,data_label[1][1].find_first_of("("));

    if(plot_type == 2){
        ofstream mean_file("mean.txt",ios::app);

        mean_file<<"--------------------------"<<out_fig_name<<" win size:"<<win_size<<endl;
        for (int file_idx = 0; file_idx < file_num; ++file_idx)
        {
            for(auto& it:x_data[file_idx]){
                it -= *smallest;
            }

            double sum = std::accumulate(std::begin(y_data[file_idx]), std::end(y_data[file_idx]), 0.0);
            double mean =  sum / y_data[file_idx].size(); //均值
            mean_file << "mean of "<< data_label[file_idx][2] <<" "<< data_label[file_idx][1] << " is " << mean << endl;
        }
        mean_file<<endl;
        mean_file.close();
    }

    if(plot_type == 2){
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

        plt::save(out_fig_name+"_point_win"+to_string(win_size)+".svg");
        plt::show();

    }else if(plot_type == 1){
        plt::subplot(1, 1, 1);
        for (int file_idx = 0; file_idx < file_num; ++file_idx)
        {
            plt::named_plot(data_label[file_idx][2],
                            x_data[file_idx],
                            y_data[file_idx]);
//                            data_label[file_idx][3] + data_label[file_idx][4] + data_label[file_idx][5]);

            plt::plot(x_data[file_idx], y_data[file_idx], data_label[file_idx][3] + data_label[file_idx][4]);
            plt::xlabel(data_label[file_idx][0]);
            plt::ylabel(data_label[file_idx][1]);
            plt::legend();
        }

        plt::save(out_fig_name+"_line_win"+to_string(win_size)+".svg");

        plt::show();
    }

}