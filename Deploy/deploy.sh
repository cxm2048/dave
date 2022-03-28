#!/bin/bash
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# */

chmod a+x *.sh curl

PROJECT=demo
GPU='"NULL"'
IMAGE=ubuntu_docker_image
TAG=latest
EXTEND=''
PROJECTMAPPING=''
USERNAME=${USER}
JUPYTERPORT=8888
HOMEPATH='./'

while getopts ":p:g:i:t:e:h:j:" opt
do
    case $opt in
        p)
        PROJECT=$OPTARG
        echo PROJECT:$PROJECT
        ;;
        g)
        GPU=$OPTARG
        echo GPU:$GPU
        ;;
        i)
        IMAGE=$OPTARG
        echo IMAGE:$IMAGE
        ;;
        t)
        TAG=$OPTARG
        echo TAG:$TAG
        ;;
        e)
        EXTEND=$OPTARG
        PROJECTMAPPING=$(echo $EXTEND | grep -o "project:/project")
        echo EXTEND:$EXTEND
        echo PROJECT MAPPING:$PROJECTMAPPING
        ;;
        h)
        HOMEPATH=$OPTARG
        echo HOME PATH:$HOMEPATH
        ;;
        j)
        JUPYTERPORT=$OPTARG
        echo JUPYTER PORT:$JUPYTERPORT
        ;;
        ?)
        echo "未知参数:" $opt
        ;;
    esac
done

if [ "$USERNAME" == "root" ]; then
   PROJECTNAME=${PROJECT}
else
   PROJECTNAME=${PROJECT}-${USERNAME}
fi

if [ -f environment.sh ]; then
   ./environment.sh
fi

#
# 如果容器时存在的，
# 后续的流程需要这个容器是处于开机状态
#
./booting.sh $PROJECTNAME

if [ -f load.sh ]; then
   ./load.sh ${IMAGE} ${TAG}
elif [ -f dockerimage.sh ]; then
   ./dockerimage.sh $PROJECT ${IMAGE} ${TAG}
fi
if [ $? != 0 ]; then
   exit $?
fi

./remove.sh $PROJECTNAME $PROJECT ${IMAGE} ${TAG}

#
# 使用${IMAGE}的镜像构建一个名叫${PROJECTNAME}的容器，
# 并启动容器，
# 并暴露容器内端口所有端口到容器外
# 调用update.sh更新工程文件
#

exit_project_contains=`docker ps -a | grep -w "${PROJECTNAME}"`

if [ "$exit_project_contains" == "" ]; then
   if [ "$GPU" == '"NULL"' ]; then
      docker run ${EXTEND} --cap-add sys_ptrace --restart always -itd --network host --hostname ${PROJECTNAME}-docker --name ${PROJECTNAME} ${IMAGE}:${TAG}
   else
      docker run ${EXTEND} --cap-add sys_ptrace --restart always -itd --network host --hostname ${PROJECTNAME}-docker --name ${PROJECTNAME} --gpus ${GPU} ${IMAGE}:${TAG}
   fi

   ./restore.sh $PROJECTNAME $PROJECT

   ./update.sh ${HOMEPATH} ${PROJECT} ${PROJECTNAME} ${JUPYTERPORT} ${PROJECTMAPPING}
   echo Successfully created a new ${PROJECTNAME} container!
else
   ./update.sh ${HOMEPATH} ${PROJECT} ${PROJECTNAME} ${JUPYTERPORT} ${PROJECTMAPPING}
   echo The ${PROJECTNAME} container exists, no new container is installed! But update the ${PROJECTNAME}
   ./release.sh ${PROJECTNAME}
fi