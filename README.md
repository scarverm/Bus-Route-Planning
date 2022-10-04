## 公交路线规划
# 任务要求

1. 建议使用Visual Studio Community 2015及以上版本进行开发，采用C++或者C#语言实现，可以使用.Net Framework，运行环境为64-bit Windows 10。

2. 提交的代码要求经过Code Quality Analysis工具的分析并消除所有的警告。Code Quality Analysis工具的用法参见：http://msdn.microsoft.com/en-us/library/dd264897.aspx

3. 使用性能分析工具来找出代码中的性能瓶颈并改进。

4. 写出至少10个测试用例确保程序能够正确处理各种情况。

# 作业需求

1. 把所有公交线路（编码）、站点等用一个文本文件（假设名字叫 busroute.txt）的形式保存起来，应保存的信息应包括线路名称、各个站点的名称，使得应用程序可以通过读取这个文件，就能掌握关于海口公交线路的所有信息。设计一种文件格式用于保存信息并方便应用程序读取，并说明采取这种格式的理由。为了简单起见，所有路线的站点均保持一致（自行处理），所有标注“夜*”路线都排除在外。

2. 实现一个命令行程序（hkbus.exe），这个程序启动时会读取busroute.txt中的信息，然后等待用户的输入，用户可以输入公交线路的名称，然后程序就输出此公交线路的所有站点名称（按某一方向顺序输出即可）。输出站点名称后，程序再次进入等待状态。

3. 程序在读取busroute.txt中信息时，不仅能够处理格式正确的文件，还能够处理各种异常的情况。

4. 在命令行中以-b参数加两个站点名称执行程序时，例如hkbus.exe -b station1 station2，将计算从第一个站点station1到第二个站点station2的最短（经过的站点数最少）路线，并返回经过的站点的个数和路径，如果有换乘，请列出换乘的线路。

5. 在命令行中以-c参数加两个公交站点名称执行程序时，例如hkbus.exe -c station1 station2将计算从第一个站点station1到第二个站点station2的换乘最少的最短路线，并返回经过的站点的个数和路径，如果有换乘，请列出换乘的线路。
