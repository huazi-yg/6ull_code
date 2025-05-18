# 挂载网络文件系统 (NFS) 指南

本文档记录了在开发板上挂载 Ubuntu NFS 文件系统的步骤和命令。

## 前提条件

1.  **Ubuntu 服务器 IP 地址**: `192.168.31.106`
2.  **网络连通性**: 确保开发板能够 `ping` 通 Ubuntu 服务器 `192.168.31.106`。

## 挂载命令

在开发板的终端中执行以下命令来挂载 NFS：

```bash
mount -t nfs -o nolock,vers=3 192.168.31.106:/home/book/nfs_rootfs /mnt
