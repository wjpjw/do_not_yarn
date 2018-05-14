# Do-Not-Yarn

提供gpu集群监控、TensorFlow推理任务调度的基于容器的分布式系统。

## 基于container的组件设计
Do-Not-Yarn是一个由若干服务组件组成的Kubernetes Service，它会统一暴露各个组件值得暴露的端口作为外界的访问接口。统一维护稳定的项目版本就应该以Do-Not-Yarn为基准，不允许内部组件不同版本混用（那样各个组件版本数目相乘，几乎是不可测试的）。

1. monitor container：以容器形式部署于k8s集群中的某个结点，它最合理的位置应该使它连接到所有gpurealtime的网络延时最低。C++实现。
2. gpurealtime：以程序形式部署于各个gpu服务容器的内部。C++实现。
3. dashboard server container：以容器形式部署在k8s集群中，gpu监控网站的后端，Nginx+Tornado异步服务器，用websocket和浏览器实现长连接异步信息推送，用Python的zeromq binding 订阅 monitor 的信息。
4. inference container：推理服务容器，部署在k8s集群中的gpu服务器上，用flask写的简单web服务，根据不同的REST request调用tensorflow inference routine，占用所在容器gpu进行运算。
5. scheduler container：任务调度容器，部署在monitor附近，它同样是monitor的订阅者，把不同的任务分配到不同的inference container上去。用Tornado实现request转发。
6. AI platform：AI平台后端，用spring写的整合型的web后端，用户管理之类的功能。它可以部署在任何地方，与Do-Not-Yarn耦合较低。
7. 各种各样的web前端：和Do-Not-Yarn耦合较低，只需要考虑与相应的web后端交互。其中新开发的前端可以用Stateless框架。
8. Do-Not-Yarn console: Do-Not-Yarn组件管理工具。允许更改各组件的状态、配置。

涉及web服务的组件都用python写，涉及异步网络通信的用Tornado，简单web服务用flask。核心功能都用C++实现，确保高性能。



## 非功能性需求分析
### 高可用设计
1. 为了避免单点故障，做简单的“主-从”灾备。备份的粒度依然是Do-Not-Yarn服务，整体复制一份，不必。
2. monitor充当gpurealtime的守护进程，监听其心跳，一旦超时就复活之。


### 一致性特征：AP系统
AP系统在子系统失联后依然保证最终一致性和高可用。CA系统牺牲分区容忍，要求子系统时时刻刻保持联系（否则无法处理请求），保证强一致性。AP

Do-Not-Yarn系统级的共享数据只有gpuinfo，各组件本身无状态（或者说只有生死两种状态），有问题重启即可。（此前设想过让系统记录哪个用户哪个任务在哪个gpu上跑，不过这个功能似乎没什么用，所以简化了设计。）

Do-Not-Yarn在某个gpurealtime失联后不应失去功能，应该继续装作没有什么事发生。把那个GPU的状态维持在最后一次联系的状态。


### Do-Not-Yarn服务的横向扩容
以后再考虑这个问题。

