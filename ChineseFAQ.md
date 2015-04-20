### 我如何使用这个扩展？ ###
你需要先安装一些喜欢的下载器，如迅雷，快车等，然后安装这个扩展。在选项页面中，设置默认的下载器，这样当你用鼠标点击一些下载资源的时候（我们目前只是通过链接资源的后缀进行判断），我们就会使用默认的下载器进行下载。你也可以在该链接上点右键选择下载助手菜单进行下载。在页面的空白处点右键，可以进行下载所有链接的操作。

### 如何设置默认下载器？ ###
在Option页面中选择默认的下载器，点击保存并关闭按钮。

### 为什么我设置了默认下载器，但还是使用chrome的内部下载？ ###
这是因为你点击的链接没有被识别为可下载链接。目前我们是根据后缀名来识别可下载链接的。

### 为什么我安装了下载器，但是右键下载列表中没有出现下载器呢？ ###
请尝试重启浏览器。

### 我使用多台计算机，为什么其他扩展可以同步，而这个扩展不可以同步？ ###
因为谷歌浏览器的限制，所有包含插件的扩展程序目前都不能进行同步。

### 为什么下载全部链接中没有Internet Download Manager？ ###
因为Internet Download Manager提供的COM接口只能针对IE来使用，所以我们不能进行支持。

### 是否可以提供一个接口，让其他扩展也可以调用迅雷等下载器呢？ ###
可以的，我们提供的插件dll就可以实现这个要求。使用的方法非常简单：首先在扩展中加入这个download\_helper.dll，MIMEType=‘application/x-npdownload’，你就可以使用这个CheckObject方法并且传入ProgID来检查是否存在下载器。你还可以使用CreateObject方法并传入ProgID来创建下载器对象。在调用下载器对象的方法时，目前只支持一些简单类型的参数，包括字符串，数值和数组。以迅雷为例：

```
<embed type='application/x-npdownload' id='obj1'></embed>
<script language="javascript">
  var pluginobj = document.getElementById('obj1');
  var thunderobj = pluginobj.CreateObject("ThunderAgent.Agent");
  thunderobj.AddTask(url, filename, '', comments, referurl, -1, 0, -1);
  thunderobj.CommitTasks();
</script>
```