---
layout: solution
title: Solution-安全与密码学
solution: true
index: 9
---
<head>
    <script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>
    <script type="text/x-mathjax-config">
        MathJax.Hub.Config({
            tex2jax: {
            skipTags: ['script', 'noscript', 'style', 'textarea', 'pre'],
            inlineMath: [['$','$']]
            }
        });
    </script>
</head>

### 1. 熵
假设一个密码是从四个小写的单词拼接组成，每个单词都是从一个含有10万单词的字典中随机选择，且每个单词选中的概率相同。 一个符合这样构造的例子是 `correcthorsebatterystaple` 。这个密码有多少比特的熵？
假设另一个密码是用八个随机的大小写字母或数字组成。一个符合这样构造的例子是 `rg8Ql34g` 。这个密码又有多少比特的熵？
```
Entropy = log_2(100000^4) = 66 #correcthorsebatterystaple
Entropy = log_2((26+26+10)^8) = 48 #rg8Ql34g
```
也可以使用下面两个网址进行计算
- [Calculating Password Entropy](https://www.pleacher.com/mp/mlessons/algebra/entropy.html)
- [Strength Test](http://rumkin.com/tools/password/passchk.php)

哪一个密码更强？
假设一个攻击者每秒可以尝试1万个密码，这个攻击者需要多久可以分别破解上述两个密码？

第一个更强。

因为每个密码的概率是相同，所以平均而言，需要的破解时间约为尝试所有可能密码所花时间的一半（概率论期望值的简单应用，记所有可能密码个数为x,每年尝试的密码数为y,破解所需的平均时间为：$\frac{1}{x\cdot y}(1+2+\cdots +x)=\frac{1}{x}\times \frac{(1+x)\times x/2}{y}=\frac{1}{2}\times\frac{1+x}{y}$ ）

对于第一个密码：$100,000^4/10^4/(365\times 24\times 3600)/2=1.085亿年$

对于第二个密码：$62^8/10^4/(365\times 24\times 3600)/2=346年$

### 2.密码散列函数 
从[Debian镜像站](https://www.debian.org/CD/http-ftp/)下载一个光盘映像（比如这个来自阿根廷镜像站的[映像](http://debian.xfree.com.ar/debian-cd/10.2.0/amd64/iso-cd/debian-10.2.0-amd64-netinst.iso)）。使用`sha256sum`命令对比下载映像的哈希值和官方Debian站公布的哈希值。如果你下载了上面的映像，官方公布的哈希值可以[参考这个文件](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/SHA256SUMS)。
```bash
curl -O -L -C - http://debian.xfree.com.ar/debian-cd/current/amd64/iso-cd/debian-mac-10.9.0-amd64-netinst.iso
curl -O https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/SHA256SUMS
```
注意这里 SHA256SUMS 包含的版本号，下载的 iso 文件也要是对应的版本号。原文中是 10.2.0版本，这里我们下载的是 10.9.0。当您看到此文时，可能版本又更新了，请到[这里](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/)下载最新版本。

接下来进行哈希比对：
```bash
cat SHA256SUMS | grep debian-10.9.0-amd64-netinst.iso
8660593d10de0ce7577c9de4dab886ff540bc9843659c8879d8eea0ab224c109  debian-10.9.0-amd64-netinst.iso

shasum -a 256 debian-10.9.0-amd64-netinst.iso
8660593d10de0ce7577c9de4dab886ff540bc9843659c8879d8eea0ab224c109  debian-10.9.0-amd64-netinst.iso
```
可以看出来两个哈希是一样的。
也可以用`diff`直接比较：
```bash
diff <(cat SHA256SUMS |grep debian-10.9.0-amd64-netinst.iso) <(shasum -a 256 debian-10.9.0-amd64-netinst.iso)
```

### 3. 对称加密 
使用 OpenSSL的AES模式加密一个文件: `openssl aes-256-cbc -salt -in {源文件名} -out {加密文件名}`。 使用`cat`或者`hexdump`对比源文件和加密的文件，再用 `openssl aes-256-cbc -d -in {加密文件名} -out {解密文件名} `命令解密刚刚加密的文件。最后使用`cmp`命令确认源文件和解密后的文件内容相同。
```bash
echo "hello world" > afile #创建一个文件
openssl aes-256-cbc -salt -in afile -out secfile #加密文件
enter aes-256-cbc encryption password:***
Verifying - enter aes-256-cbc encryption password:***
```
使用 `hexdump`比较两个文件
```bash
diff <(hexdump afile) <(hexdump secfile)

1,2c1,3
< 0000000 68 65 6c 6c 6f 20 77 6f 72 6c 64 0a
< 000000c
---
> 0000000 53 61 6c 74 65 64 5f 5f cb 18 f1 7c 4f 09 b4 21
> 0000010 d2 f1 56 77 80 01 b1 2f ac 2d 32 35 52 23 2e c6
> 0000020
```
使用`cat`比较两个文件
```bash
cat afile
# hello world
cat secfile
# Salted__|O      !Vw/-25R#.%
```
对文件进行解密
```bash
openssl aes-256-cbc -d -in secfile -out notsafefile

cmp afile notsafefile
$?
# 返回0,表示这两个文件内容相同
```



### 4.非对称加密
1. 在你自己的电脑上使用更安全的 [ED25519](https://wiki.archlinux.org/index.php/SSH_keys#Ed25519) 算法生成一组 [SSH 密钥对](https://wiki.archlinux.org/index.php/SSH_keys#Ed25519)。为了确保私钥不使用时的安全，一定使用密码加密你的私钥。
    ```bash
    ssh-keygen -t ed25519 
    ```
2. [配置GPG](https://www.digitalocean.com/community/tutorials/how-to-use-gpg-to-encrypt-and-sign-messages)。
Linux 系统可以直接按照上面的教程操作，MacOS 上的操作过程如下
    ```bash
    $ brew install gpg
    $ gpg --gen-key
    ```
1. 给Anish发送一封加密的电子邮件（[Anish的公钥](https://keybase.io/anish)）。

    导入Anish发布的公钥，并完成验证
    ```shell 
    ~ $ curl https://keybase.io/anish/pgp_keys.asc | gpg --import   

    ~ $ gpg --list-keys
    # ~/.gnupg/pubring.kbx中除了刚才创建的自己的公钥，还存入了Anish的公钥

    ~ $ gpg --edit-key "Anish Athalye"  # 进入gpg控制台
    gpg> fpr
    pub   rsa4096/C3F6E4F5086B3B32 2014-10-30 Anish Athalye <me@anishathalye.com>
    主密钥指纹： 72EE 4824 FA6E FF1F E750  A015 C3F6 E4F5 086B 3B32
    # 将这个密钥指纹与Anish发布在网站上的做匹配，一致
    ```
    接下来，使用Anish的公钥加密信息
    ```shell
    ~ $ touch message.txt
    ~ $ echo 'hello' > message.txt
    ~ $ gpg --encrypt --sign --armor -r me@anishathalye.com message.txt
    # 在本目录下会生成一个message.txt.asc的加密文件
    # 这个文件，只有拥有私钥的Anish能解密

    # 如果你也想查看这个加密文件，需要使用：
    ~ $ gpg --encrypt --sign --armor -r me@anishathalye.com -r usrname@example.com message.txt   
    # 添加第二个 -r 接收人为你自己，你就能解密这个文件
    ~ $ gpg message.txt.asc  
    ```        
    接下来，你就可以尝试用邮件或其他方式，将这个使用Anish的公钥加密的文件发送给他（这里只是做说明，不推荐发送无意义的信息！）
2. 使用`git commit -S`命令签名一个Git提交，并使用`git show --show-signature`命令验证这个提交的签名。或者，使用git tag -s命令签名一个Git标签，并使用`git tag -v`命令验证标签的签名。
    ```bash
    git commit -S -m "sign a commit"
    ```
    可能会出现下面的错误
    ```bash
    error: gpg failed to sign the data
    fatal: failed to write commit object
    ```
    这个问题网上有一些解决方案可供参考：
    - [Git error - gpg failed to sign data](https://stackoverflow.com/questions/41052538/)
    - [Set up Keybase.io, GPG & Git to sign commits on GitHub](https://github.com/pstadler/keybase-gpg-github)

    如果遇到
    ```bash
    echo "test" | gpg --clearsign
    gpg: [stdin]: clear-sign failed: Inappropriate ioctl for device
    ```
    则执行
    ```bash
    export GPG_TTY=$(tty)
    ```
    此外要注意`~/.gitconfig`里面的 name，email 要和生成秘钥时使用的一样，签名算法也是一样的： 
    ```bash
    gpg -K --keyid-format SHORT
    /Users/lingfeng/.gnupg/pubring.kbx
    ----------------------------------
    sec   rsa2048/56EF5DE3 2021-05-15 [SC] [有效至：2023-05-15]
        35A0BAB790EBBFE193422975097FC49956EF5DE3
    uid         [ 绝对 ] hanxiaomax-mac <hanxiaomax@qq.com>
    ssb   rsa2048/55FB9195 2021-05-15 [E] [有效至：2023-05-15]
    ```
    对应的 `.gitconfig`为
    ```
    [user]
        name = hanxiaomax-mac
        email = hanxiaomax@qq.com
        signingKey = 55FB9195
    ```
    所有问题都解决后，正常签名
    ```bash
    git commit -S -m "sign a commit"
    [main fc8e916] sign a commit
    3 files changed, 3 insertions(+)
    create mode 100644 security/afile
    create mode 100644 security/notsafefile
    create mode 100644 security/secfile
    ```
    ![1.png]({{site.url}}/2020/solutions/images/9/1.png)

    创建一个带签名的标签，并验证
    ```shell
    ~/signgit (main) $ git tag -s "v0.0.0" -m "Create a tag"

    ~/signgit (main) $ git tag -v v0.0.0
    object 03fa307f9ced5d9717473a429d76122e46c7a70f
    type commit
    tag v0.0.0
    tagger usrname <usrname@example.com> 1683883065 +0800

    Create a tag
    gpg: Signature made Fri May 12 17:18:13 2023 CST
    gpg:                using RSA key 07C700F1B0F8A6BFDA61571CA29FF423FCBD1279
    gpg:                issuer "usrname@example.com"
    gpg: Good signature from "usrname <usrname@example.com>" [ultimate]
    ```
