###chatty是我打算一直维护的一个聊天软件。

####目标：
做成一个并发聊天服务器，可以让客户端与客户端聊天而不是客户端和服务端聊天。
具有单个人聊天功能和聊天室功能。
传输内容加密，openssl。

####v0.1:
目前的功能：
客户端可服务器端可互相通信，且都是发出消息后，需要等待对方回复才能继续发送消息。

###v0.2:
比较服务器多线程和多进程方案，实现双方可以一边发送一边接受消息。
问题：
####ISSUE:
在client端输入quit后，写线程结束，关闭了socket，但是读线程依然没有关闭。必须在接收一次server端的send后，因为socket已关，导致len<0，然后才能退出。
####原因：
当server输入quit时，client的读线程是主线程，主线程会break导致整个程序退出，于是client的socket关闭。当client输入quit时，server的读线程退出，紧接着的while循环并不退出。所以server的socket并没有关闭。
####解决办法：
只要在server的handle_read方法里，break前close掉socket即可。

*另外：发现了vim的一个插件accelerated-smooth-scroll，可以实现vim翻页时候的滚动效果，并非瞬间展示。用法，先make，然后在~/.vim目录下解压对应的zip即可。*


*参考：*

[《APUE》]()、[并发服务器模型：单客户端单线程，统一accept](http://blog.chinaunix.net/uid-8196371-id-1676942.html)、[用C实现的聊天程序](http://blog.csdn.net/zx824/article/details/7752894)
