---
layout: solution
title: Solution-编辑器 (Vim)
solution: true
index: 3
---

1. 完成 vimtutor。 备注： 它在一个 80x24（80 列，24 行） 终端窗口看起来最好。
  ```bash
  vimtutor
  ```
2. 下载我们的[vimrc](https://missing-semester-cn.github.io/2020/files/vimrc)，然后把它保存到 `~/.vimrc`。 通读这个注释详细的文件 （用 Vim!）， 然后观察 Vim 在这个新的设置下看起来和使用起来有哪些细微的区别。
3. 安装和配置一个插件： `ctrlp.vim`.
   1. 用 `mkdir -p ~/.vim/pack/vendor/start` 创建插件文件夹
   2. 下载这个插件： `cd ~/.vim/pack/vendor/start; git clone https://github.com/ctrlpvim/ctrlp.vim`   
   下载后需要在~/.vimrc 中添加如下设置，参考[这里](http://ctrlpvim.github.io/ctrlp.vim/#installation)
       ```vim
       set runtimepath^=~/.vim/pack/vendor/start/ctrlp.vim 
       ```
   1. 请阅读这个插件的[文档](https://github.com/ctrlpvim/ctrlp.vim/blob/master/readme.md)。 尝试用 CtrlP 来在一个工程文件夹里定位一个文件， 打开 Vim, 然后用 Vim 命令控制行开始 :CtrlP.  ![1.png]({{site.url}}/2020/solutions/images/3/1.png)
   2. 自定义 CtrlP： 添加 [configuration](https://github.com/ctrlpvim/ctrlp.vim/blob/master/readme.md#basic-options) 到你的 ~/.vimrc 来用按 Ctrl-P 打开 CtrlP
       ```vim
       let g:ctrlp_map ='<c-p>' 
       let g:ctrlp_cmd = 'CtrlP'
       let g:ctrlp_working_path_mode = 'ra' #设置默认路径为当前路径
       ```
       ![1.png]({{site.url}}/2020/solutions/images/3/2.png)
4. 练习使用 Vim, 在你自己的机器上重做演示。
5. 下个月用 Vim 完成_所有_的文件编辑。每当不够高效的时候，或者你感觉 “一定有一个更好的方式”， 尝试求助搜索引擎，很有可能有一个更好的方式。如果你遇到难题， 来我们的答疑时间或者给我们发邮件。
6. 在你的其他工具中设置 Vim 快捷键 （见上面的操作指南）。
7. 进一步自定义你的 ~/.vimrc 和安装更多插件。
  安装插件最简单的方法是使用 Vim 的包管理器，即使用 vim-plug 安装插件：
   1. 安装 vim-plug
    ```bash
    $ curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
      https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
    ```
   2. 修改 ~/.vimrc
    ```vim
    call plug#begin()
    Plug 'preservim/NERDTree' #需要安装的插件 NERDTree
    Plug 'wikitopian/hardmode'  #安装 hardmode
    ..... # 更多插件
    call plug#end()
    ```
   3. 在 vim 命令行中执行 `:PlugInstall`
 ![1.png]({{site.url}}/2020/solutions//images/3/3.png)
8. (高阶)用 Vim 宏将 XML 转换到 JSON ([例子文件](https://missing-semester-cn.github.io/2020/files/example-data.xml))。 尝试着先完全自己做，但是在你卡住的时候可以查看上面 [宏](https://missing-semester-cn.github.io/2020/editors/#macros) 章节。  

   可以先查看[转化后的JSON文件]({{site.url}}/2020/solutions/demoCode/3/example-data.json)，了解最终的转换效果。
   ```bash
   vim example-data.xml
   ```
   在`vim`编辑页面中执行以下步骤：

   1.  删除首尾两行         
    *    `Gdd`：跳转到最后一行，并删除该行        
    *    `ggdd`： 跳转到第一行，并删除该行

   2.  录制寄存器`e`，实现对`<name>`标签的处理 
    *    `/<name>`，再键入`Enter`，然后键入`N`：查找`<name>`并跳转到文件的最后一个`<name>`
    *    接下来，录制宏（即寄存器`e`）：        
         *    `qe`：即将录制名为`e`的宏       
         *    `^r"`：`^`跳转到当前行的首个非空字符，即`<name>`的`<`，`r"`将`<`替换为`"`       
         *    `f>s": "`：`f>`查找`>`，此处即匹配刚才的`<name>`的`>`；`s"`将`>`替换为`": "`      
         *    `<Esc>`：回到正常模式       
         *    `f<C"`：查找下一个`<`，由于xml文件的特征，此时匹配到的是刚才修改的`<name>`对应的`</name>`的`<`；然后，将当前位置到本行末尾的内容删除，同时插入`"`       
         *    `<Esc>`：回到正常模式       
         *    `q`：停止录制，寄存器`e`完成
    *    寄存器`e`的效果示例：将`<name>Mike</name>`修改为`"name": "Mike"`  

   3.  录制寄存器`p`，实现对`<person>`标签的处理
   *   `gg`：跳转到文件开头（本例中，正好处于一个`<person>`标签）
   *   接下来，录制宏（寄存器`p`）
       *   `qp`：即将录制宏`p`
       *   `S{`：将当前行删除，并替换为`{`
       *   `<Esc>`：回到正常模式
       *   `j@e`：移到下一行，并执行宏`e`（因为本例的xml文件中，`<person>`的下一行为`<name>`）
       *   `A,`：在本行的末尾添加上`,`
       *   `<Esc>`
       *   `j@e`：移到下一行，并执行宏`e`（本例中，完成对`<email>`标签行的处理，其处理与`<name>`标签行的一致）
       *   `jS},`：跳转到下一行，并删除整行，同时添加上`},`（本例中该行内容为：`</person>`）
       *   `<Esc>q`：回到正常模式，并停止录制，完成宏`p`
   4.  录制寄存器`q`，实现从一个`<person>`标签到另一个`<person>`标签的跳转
   *   `/<person>`：从第三步的位置开始查找下一个`<person>`标签
   *   `qq`：开始录制宏`q`
   *   `@p`：执行宏`p`，可以将这一组`<person>`标签包含的内容修改为json格式
   *   `jq`：移到下一行，并停止录制，完成宏`q`（此时，刚好处于下一个`<person>`标签行）
   5.  执行宏`q`到文件尾
   *   `999@q`：执行999次宏`q`（本文档总行数为1002行，执行999次能确保到达文件末尾）
   6.  手动修改文件内容
   *   `gg`
   *   `O`：（大写的`o`）在开头插入如下内容
       ```shell
       {
           "people": {
               "person": [
       ```
   *   `<Esc>`
   *   `/<email>`：跳转到文件末尾唯一未处理的`<email>`标签行
   *   `@e`：对该标签行进行处理
   *   `jS}`：移到下一行，删除`</person>`标签
   *   `<Esc>`
   *   `o`：在文件末尾插入以下内容
       ```shell
               ]
           }
       }
       ```
