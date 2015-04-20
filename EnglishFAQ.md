### How to use this extension? ###
Before using this extension, you need to have at lease one downloader installed, eg. FlashGet, Thunder, etc. You can change the default downloader to your favorite downloader in the option page, so that when you click on a downloadable link (we identify downloadable links from their file extensions), the default downloader will start. Or you can right click on any link in the page and choose a downloader from Download Helper menu to download that link. You can also right click on the blank area of the page and choose a downloader to download all links in the page.

### How to change the default downloader? ###
Select default downloader on the option page, and click on “Save & Close”.

### I’ve changed the default downloader. Why did my download still go to the internal downloader instead of my default downloader? ###
It’s because the link you clicked is not identified as a downloadable link. We identify downloadable links from their file extensions.

### I’ve installed a new downloader. Why can’t I find it in Download Helper menu? ###
Please try restarting your browser.

### Why can’t I sync this extension access multiple computers? ###
This extension contains an NPAPI plugin, so it can’t be synced.

### Why don’t you support using Internet Download Manager to download all links on the page? ###
The COM interface Internet Download Manger provides is only applicable to IE, so we can’t use it.

### Can you provide an API for other extensions to execute Thunder? ###
Yes. You can download our extension, unzip it, find download\_helper.dll in the plugin directory, and use it as an NPAPI plugin in your extension. The MIMEType is “application/x-npdownload”. You can use CheckObject(prog\_id) to check if a downloader has been installed. You can use CreateObject(prog\_id) to create a downloader object, and call its functions to start download. Now we only support strings, integers and arrays as parameters when calling functions of the downloader object. Take Thunder as an example:

```
<embed type='application/x-npdownload' id='obj1'></embed>
<script language="javascript">
  var pluginobj = document.getElementById('obj1');
  var thunderobj = pluginobj.CreateObject("ThunderAgent.Agent");
  thunderobj.AddTask(url, filename, '', comments, referurl, -1, 0, -1);
  thunderobj.CommitTasks();
</script>
```