# Tips in C

## 1.使用getchar()读取输入时把局部变量ch定义为int型(int ch)

* getchar函数返回的是int型的值,而不是char型
* 当不能连续读入时(一般是因为碰到了文件的末尾),getchar返回值EOF

## 2.scanf函数读入字符时把控制符定义为" %c"

%c前加上一个空格,程序会屏蔽空白字符,从输入流中的第一个非空白字符开始读取