# Git快速入门

1. 下载安装Git
2. 在Github创建一个账号
3. 使用**git clone**把仓库克隆到本地
   * eg：**git clone 'ssh地址'**

4. 在工作区内进行创建或修改
5. **git add**：把文件加入到暂存区
   * eg：**git add .**  将全部文件加入到暂存区
6. **git commit**:把暂存区的文件加入到版本库
   * eg：**git commit -m "description"**
   * eg: **git commit -am "description"**    直接提交修改,免去add步骤
7. **git tag**: 给当前的版本起标签
   * eg: **git tag 标签名称**
8. **git log**:查看历史版本信息
9. **git checkout 分支/版本名/Tag**: 切换Head所指向的版本
10. **git reset --hard**:回溯版本
    * eg：**git reset --hard 版本号/Tag**
11. **git status**:查看当前工作区的状态
12. **git push**:把更新推送到远程仓库
13. **git pull**:把远程仓库拉到本地