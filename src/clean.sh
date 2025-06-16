#!/bin/sh

rm -rf $(find ./ -name "debug" -or -name "release" -and -type d)
rm -rf $(find ./ -name "Debug" -or -name "Release" -and -type d)
rm -rf $(find ./ -name ".svn" -and -type d)
rm -rf $(find ./ -name "*‾")
rm -rf $(find ./ -name "*.suo")
rm -rf $(find ./ -name "*.ncb")
rm -rf $(find ./ -name "*.user")
# 下列操作只在src下执行
rm -rf $(find ./ -name "*.lib")
rm -rf $(find ./ -name "*.pdb")
rm -rf $(find ./ -name "*.idb")
