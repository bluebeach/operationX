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


