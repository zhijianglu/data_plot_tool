# data_plot_tool
==
基于matplot-cpp的绘图工具，对算法实时性准确性输出固定格式的txt数据文件，可对比多个数据文件并且绘图，输出均值，滑窗累加。
数据格式:

![Alt text](https://github.com/zhijianglu/data_plot_tool/blob/master/pics/%E5%9B%BE%E7%89%871.png)

使用plot_tool工具有几项输入参数：
usage: 
        ./data_plot data1.txt   data2.txt    data3.txt   plot_type   win_size
可以显示多个数据，数据的第一行用以下格式指明横轴纵轴：
plot_type： 使用划线模式：1 或者使用画点模式：2
win_size：  时间积分窗口大小
只要修改数据里面的头行就可以修改坐标轴的名称了，但是注意一定要格式标准，#号后面有空格，多个单词用下划线隔开别用空格  最后一个单词指明你希望在图上显示的曲线名称。
如需要配置线的颜色和点，就需要在第二行用以下格式指出：
* line_type point_type color
Linetype就是划线类型，实线或者波浪线或者虚线，point_type是点类型可以是圆点三角形五角星等等，在matplot官方说明有，可查阅：

[matplot](https://www.cnblogs.com/HuZihu/p/9430406.html)
                
如果不在数据文件中指出绘图类型的话，就会按照输入文件的顺序，配置以下绘图类型，一般默认即可：

                    pre_colors  =  {"b","g","r","c","m","y","k","b","g","r","c","m","y","k"};
                    
                    pre_lines   =  {"-","-","-","-","-","-","-",":",":",":",":",":",":",":"};
                    
                    pre_points  =   {"^","1","2","s","p","*","h","+","x","D","d","v","^","o"};
                    

该绘图工具采用的是matplotlib的c++版本，叫做matplotlib-cpp，github仓库为：

[matplotlib-cpp](https://github.com/lava/matplotlib-cpp)
                
该绘图工具使用起来比较简单，且可以嵌入到c++程序之中，实时出图。安装numpy和python，和使用pip安装一下 matplotlib的python版本在电脑即可。
