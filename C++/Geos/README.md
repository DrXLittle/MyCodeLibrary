# 简介

这里放的是一些和geo相关的C++文件，毕竟收集一些C++代码还是很不容易的

# 文件介绍

## find_end_point.cpp

这个文件，输入可以有两种：
1. 两个点A、B的经纬度坐标
2. 点A的经纬度坐标、距离d、方位b

对于第1种输入，是会先去计算A、B间的距离以及方位，然后再以点A经纬度坐标、距离、方位为输入，得到终点坐标，与B点进行比对。

对于第2种输入，就是正常的做法啦，不再赘述。

关于这份代码，非常感谢两个网址：

https://stackoverflow.com/questions/32096968/finding-earth-coordinates-latitude-longitude-distance-meters-and-bearing-a
https://ideone.com/9yuONO

以及，关于两点间距离，或者根据距离求终点等等的运算，在下面这个网址有很好的介绍，而且有可视化：
https://www.movable-type.co.uk/scripts/latlong.html?from=49.1715000,-121.7493500&to=49.18258,-121.75441

不过，这份网址里的关于计算终点坐标的代码，没有C++版的，我将其JavaScript版本的修改成C++后，总感觉怪怪的，所以就没再用它的代码了。
