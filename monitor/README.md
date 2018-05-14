# GPU-Monitor
收集各GPU容器推送信息的监控组件，提供全局GPU状态概览。

## AutoSocket 
### 心跳
每个autosocket有3滴血，每次poll hit就回满，每次miss就掉一滴血。没血了就判定是死了，通知k8s复活对应的gpurealtime进程。

### callback封装
对信息的处理封装在callback中，callback是function对象。每次hit就调用处理gpuinfo的function对象。

### 数据源id
用ip:port识别数据源。不区分pid和启动日期，因为gpurealtime是无状态的，前一秒挂了，后一秒再起一个也是一样的。
