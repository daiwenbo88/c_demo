cmake_minimum_required(VERSION 3.25)
project(ipc)

set(CMAKE_CXX_STANDARD 17)
#添加头文件搜索路径
include_directories(/usr/local/include)
#添加库文件搜索路径
link_directories(/usr/local/lib)
link_libraries(event_core event_extra event_openssl event_pthreads event)
add_executable(ipc main.c pipe/pipe.h pipe/pipe.c pipe/fifo.h pipe/fifo.c pipe/mmap.h pipe/mmap.c pipe/signal.h pipe/signal.c pipe/thread.h pipe/thread.c pipe/lock.h pipe/lock.c pipe/linkList.h pipe/linkList.c socket/socket.h socket/socket.c socket/wrap.h socket/wrap.c udp/udp.h udp/udp.c socket/local_socket.h socket/local_socket.c libevent/libevent.h libevent/libevent.c web/webserver.c web/webserver.h web/log.h web/log.c web/pub.h web/pub.c)
#target_link_libraries(event_core-2.2.1 event_core event_extra-2.2.1 event_extra event_openssl-2.2.1 event_openssl event_pthreads-2.2.1 event_pthreads event-2.2.1 event)
#target_link_libraries(learn_libevent event)
