//
// Created by WMJ on 2022/6/12.
//

#include "NetworkManager.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

void RSL::NetworkManager::start() {
    running = true;
}

void RSL::NetworkManager::stop() {
    running = false;
}

void RSL::NetworkManager::networkLoop() {
    constexpr int epoll_event_size = 20;

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        printf("socket error!\n");
        return;
    }

    struct sockaddr_in serv_addr{};
    struct sockaddr_in clit_addr{};
    socklen_t clit_len;


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 0;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// 任意本地ip

    int ret = bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret == -1){
        printf("bind error!\n");
        return;
    }

    listen(sockfd,5);


    //创建epoll
    int  epoll_fd = epoll_create(epoll_event_size);
    if(epoll_fd == -1){
        printf("epoll_create error!\n");
        return ;
    }

    //向epoll注册sockfd监听事件
    struct epoll_event ev{};   //epoll事件结构体
    struct epoll_event events[epoll_event_size];  //事件监听队列

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;

    int ret2 = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sockfd,&ev);
    if(ret2 == -1){
        printf("epoll_ctl error!\n");
        return ;
    }
    socklen_t socklen = sizeof(serv_addr);
    getsockname(sockfd, (struct sockaddr*)&serv_addr, &socklen);
    networkInited.emit(serv_addr.sin_port);

    while(running){
        int nfds = epoll_wait(epoll_fd,events,epoll_event_size,-1);
        if(nfds == -1){
            printf("epoll_wait error!\n");
            return ;
        }
        printf("nfds: %d\n",nfds);
        for(int i = 0;i<nfds;++i){
            if(events[i].data.fd == sockfd){
                int connfd =  accept(sockfd,(struct sockaddr*)&clit_addr,&clit_len);
                if(connfd == -1){
                    printf("accept error!\n");
                    return ;
                }

                ev.events = EPOLLIN;
                ev.data.fd = connfd;
                if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,connfd,&ev) == -1){
                    printf("epoll_ctl add error!\n");
                    return;
                }
            }
            else{
//                char buff[BUFF_LENGTH];
//                int ret1 = read(connfd,buff,sizeof(buff));
//                printf("%s",buff);
            }
        }
    }

    close(epoll_fd);
    close(sockfd);
}
