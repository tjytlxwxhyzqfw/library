### 2016-11-30

##### lgnum.h

- 将lgnum类的名字改为大写字母开头: Lgnum,于是又有一批代码不能编译了:(
- 为Lgnum填加了析构器,但是没有测试

### 2016-11-29

##### prftree.h

修正了先前版本中存在的以下BUG:

- ~Prftree()直接删除root而非调用销毁例程clear(),你好啊,内存泄漏
- 误用delete直接删除数组
- clear()例程中,将参数rt误写成root

### 2016-11-28

##### number.h

- 增加了素数测试例程: isprime()
