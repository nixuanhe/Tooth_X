

#  Todo

- [ ] 看看硬件选型
- [ ] 然后画板子！
- [ ] 
- [ ] 
- [ ] 















# 日志

+ 20220726

+ + 写一半不行了，感觉全糅杂在一起了
  + 这里上传温度应该是16位不是8位不能这样搞
  + 温度这边数据处理还没弄好
  + 其他的应该还可以吧
  + 先上传GitHub吧
  + 感觉要先去写下位机，但是写下位机又要先画板！
  
+ 20220727

+ + 什么屎山，算了吧。随意发挥了。
  + 解决了串口错帧
  + 好像错帧并没有解决，加上isEmpty判断以后好像解决了
  + 但是不管加不加的话接收的那个bytes都会加啊！！！
  + 玄学了？
  
+ 20220728

+ + 应该算是解决错帧了吧

  + ```c++
        recBytesLenth +=bytes.length();
    
        qDebug()<<"已经接收:"<<recBytesLenth;
    
        if(false == bytes.isEmpty())
            serialDeviceRecData(bytes);
        else
        {
            qDebug()<<"empty";
        }
    ```

  + 这个if判断应该是比较重要，debug出来的结果是，输出empty的时候接收总长并不会增加，但是串口还是会去读空数据。
  
  + 完成了温度曲线显示，虽然有点丑，凑合用吧
  
  + 上位机应该先告一段落了，不知道怎么写下去了，把其他部分先弄起来！

