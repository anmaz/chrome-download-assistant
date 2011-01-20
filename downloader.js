/* Extend function  */
function extend(subClass, superClass) {
  var F = function() {};
  F.prototype = superClass.prototype;
  subClass.prototype = new F();
  subClass.prototype.constructor = subClass;

  subClass.superClass = superClass.prototype;
  if (superClass.prototype.constructor == Object.prototype.constructor) {
    superClass.prototype.constructor = superClass;
  }
}

/* superClass */
function Downloader(plugin) {
  this.plugin = plugin;
}

Downloader.prototype.checkDownloader = function() {
  return this.plugin.CheckObject(this.progId);
}

Downloader.prototype.updateNPObjectIfNeeded = function() {
  if (!this.npObject)
    this.npObject = this.plugin.CreateObject(this.progId);
}

Downloader.prototype.resetNPObject = function() {
  this.npObject = null;
}

Downloader.prototype.download = function() {}

Downloader.prototype.downloadAll = function() {}

/* Thunder */
function Thunder(plugin) {
  Thunder.superClass.constructor.apply(this, arguments);
  this.progId = 'ThunderAgent.Agent.1';
}
extend(Thunder, Downloader);

Thunder.prototype.download = function(linkObj) {
  this.npObject.AddTask5(
      linkObj.url, '', '', '', '', -1, 0, -1, '', '', '', 1, '', -1);
  this.npObject.CommitTasks2(1);
}

Thunder.prototype.downloadAll = function(links) {
  for (var i = 0; i < links.length; i++) {
    this.npObject.AddTask4(
        links[i].url, '', '', links[i].text, '', -1, 0, -1, '', '', '');
  }
  this.npObject.CommitTasks2(1);
}

/* Mini Thunder */
function MiniThunder(plugin) {
  MiniThunder.superClass.constructor.apply(this, arguments);
  this.progId = 'ToolbarThunder.DownloadAgent.1';
}
extend(MiniThunder, Downloader);

MiniThunder.prototype.download = function(linkObj) {
  this.npObject.AddTask(linkObj.url, '', '', '', '');
}

/* FlashGet */
function Flashget(plugin) {
  Flashget.superClass.constructor.apply(this, arguments);
  this.progId = 'BHO.IFlashGetNetscapeEx';
  this.version = '3.x';
}
extend(Flashget, Downloader);

Flashget.prototype.checkDownloader = function() {
  return this.plugin.CheckObject('BHO.IFlashGetNetscapeEx') ||
      this.plugin.CheckObject('JetCar.Netscape');
}

Flashget.prototype.updateNPObjectIfNeeded = function() {
  if (this.version == '3.x' && !this.plugin.CheckObject(this.progId)) {
    this.progId = 'JetCar.Netscape';
    this.version = '1.x';
    this.npObject = null;
  } else if (this.version == '1.x' &&
      this.plugin.CheckObject('BHO.IFlashGetNetscapeEx')) {
    this.progId = 'BHO.IFlashGetNetscapeEx';
    this.version = '3.x';
    this.npObject = null; 
  }
  if (!this.npObject)
    this.npObject = this.plugin.CreateObject(this.progId);
}

Flashget.prototype.download = function(linkObj) {
  this.npObject.AddUrl(linkObj.url, linkObj.text, linkObj.pageUrl);
}

Flashget.prototype.downloadAll = function(links, pageUrl) {
  var newLinks = [];
  newLinks.push(pageUrl);
  for (var i = 0; i < links.length; i++) {
    var url = links[i].url;
    var text = links[i].text;
    newLinks.push(url);
    newLinks.push(text);
  }
  if (this.version == '3.x')
    this.npObject.AddAll(newLinks, '', "FlashGet3", 0, '');
  else
    this.npObject.AddUrlList(newLinks);
}

/* Mini FlashGet */
function MiniFlashget(plugin) {
  MiniFlashget.superClass.constructor.apply(this, arguments);
  this.progId = 'BHO.IFlashGetNetscape';
}
extend(MiniFlashget, Downloader);

MiniFlashget.prototype.download = function(linkObj) {
  this.npObject.AddUrlEx(
      linkObj.url, linkObj.text, '', "FlashGetMini", 0, '');
}

MiniFlashget.prototype.downloadAll = function(links, pageUrl) {
  var newLinks = [];
  newLinks.push(pageUrl);
  for (var i = 0; i < links.length; i++) {
    var url = links[i].url;
    var text = links[i].text;
    newLinks.push(url);
    newLinks.push(text);
  }
  this.npObject.AddAll(newLinks, '', "FlashGetMini", 0, '');
}

/* QQ WhirlWind */
function QQWhirlWind(plugin) {
  QQWhirlWind.superClass.constructor.apply(this, arguments);
  this.progId = 'QQIEHelper.QQRightClick.2';
}
extend(QQWhirlWind, Downloader);

QQWhirlWind.prototype.download = function(linkObj) {
  this.npObject.SendUrl2(
      linkObj.url, linkObj.pageUrl, linkObj.text, '', 0, 0);
}

QQWhirlWind.prototype.downloadAll = function(links, pageUrl) {
  for (var i = 0; i < links.length; i++) {
    this.npObject.AddTask(links[i].url, pageUrl, links[i].text);
  }
  this.npObject.SendMultiTask();
}

/* EMule */
function EMule(plugin) {
  EMule.superClass.constructor.apply(this, arguments);
  this.progId = 'IE2EM.IE2EMUrlTaker';
}
extend(EMule, Downloader);

EMule.prototype.download = function(linkObj) {
  this.npObject.SendUrl(linkObj.url, linkObj.text, linkObj.pageUrl);
}

/* Orbit */
function Orbit(plugin, progId) {
  Orbit.superClass.constructor.apply(this, arguments);
  this.progId = 'Orbitmxt.Orbit';
}
extend(Orbit, Downloader);

Orbit.prototype.download = function(linkObj) {
  this.npObject.download(linkObj.url, linkObj.text, linkObj.pageUrl, '', 0);
}

Orbit.prototype.downloadAll = function(links, pageUrl) {
  var urls = [];
  var texts = [];
  for (var i = 0; i < links.length; i++) {
    urls.push(links[i].url);
    texts.push(links[i].text);
  }
  this.npObject.downloadList(urls, texts, pageUrl, "");
}

/* IDM */
function IDM(plugin) {
  IDM.superClass.constructor.apply(this, arguments);
  this.progId = 'DownlWithIDM.LinkProcessor';
}
extend(IDM, Downloader);

IDM.prototype.download = function(linkObj) {
  this.npObject.Download(linkObj.url);
}

/* FDM */
function FDM(plugin, progId) {
  FDM.superClass.constructor.apply(this, arguments);
  this.progId = 'WG.WGUrlReceiver';
  this.progId2 = 'WG.WGUrlListReceiver';
}
extend(FDM, Downloader);

FDM.prototype.updateNPObjectIfNeeded = function() {
  if (!this.npObject)
    this.npObject = this.plugin.CreateObject(this.progId);
  if (!this.npObjectAll)
    this.npObjectAll = this.plugin.CreateObject(this.progId2);
}

FDM.prototype.resetNPObject = function() {
  this.npObject = null;
  this.npObjectAll = null;
}

FDM.prototype.download = function(linkObj) {
  this.npObject.Url = linkObj.url;
  this.npObject.Comment = linkObj.text;
  this.npObject.Referer = linkObj.pageUrl;
  this.npObject.Cookies = "";
  this.npObject.AddDownload();
}

FDM.prototype.downloadAll = function(links, pageUrl) {
  this.npObjectAll.Referer = pageUrl;
  this.npObjectAll.Cookies = "";
  for (var i = 0; i < links.length; i++) {
    this.npObjectAll.Url = links[i].url;
    this.npObjectAll.Comment = links[i].text;
    this.npObjectAll.AddUrlToList();
  }
  this.npObjectAll.ShowAddUrlListDialog();
}

var downloaderManager = {};

downloaderManager.menuItems = [
  {
    name: 'flashget', showName: 'menu_flashget',
    showName2: 'download_all_with_flashget', privateLink: 'flashget://',
    supportDownloadAll: true, image: 'images/icon_flashget.png'
  }, {
    name: 'mini_flashget', showName: 'menu_mini_flashget',
    showName2: 'download_all_with_mini_flashget', privateLink: 'flashget://',
    supportDownloadAll: true, image: 'images/icon_flashget.png'
  }, {
    name: 'thunder', showName: 'menu_thunder', 
    showName2: 'download_all_with_thunder', privateLink: 'thunder://',
    supportDownloadAll: true, image: 'images/icon_thunder.png'
  }, {
    name: 'mini_thunder', showName: 'menu_mini_thunder',
    showName2: 'download_all_with_mini_thunder', privateLink: 'thunder://',
    supportDownloadAll: false, image: 'images/icon_thunder.png'
  }, {
    name: 'qq_whirlwind', showName: 'menu_qq_whirlwind',
    showName2: 'download_all_with_qq_whirlwind', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_qq.png'
  }, {
    name: 'emule', showName: 'menu_emule',
    showName2: 'download_all_with_emule', privateLink: 'ed2k://',
    supportDownloadAll: false, image: 'images/icon_emule.png'
  }, {
    name: 'orbit', showName: 'menu_orbit',
    showName2: 'download_all_with_orbit', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_orbit.png'
  }, {
    name: 'idm', showName: 'menu_idm',
    showName2: 'download_all_with_idm', privateLink: '',
    supportDownloadAll: false, image: 'images/icon_idm.png'
  }, {
    name: 'fdm', showName: 'menu_fdm',
    showName2: 'download_all_with_fdm', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_fdm.png'
  }, {
    name: 'chrome_downloader', showName: 'menu_chrome', isSystem: true,
    supportDownloadAll: false, image: 'images/icon_chrome.png'
  }
];

downloaderManager.downloader = {};

downloaderManager.init = function(plugin) {
  downloaderManager.downloader['thunder'] = new Thunder(plugin);
  downloaderManager.downloader['mini_thunder'] = new MiniThunder(plugin);
  downloaderManager.downloader['flashget'] = new Flashget(plugin);
  downloaderManager.downloader['mini_flashget'] = new MiniFlashget(plugin);
  downloaderManager.downloader['qq_whirlwind'] = new QQWhirlWind(plugin);
  downloaderManager.downloader['emule'] = new EMule(plugin);
  downloaderManager.downloader['orbit'] = new Orbit(plugin);
  downloaderManager.downloader['idm'] = new IDM(plugin);
  downloaderManager.downloader['fdm'] = new FDM(plugin);
}

downloaderManager.updateDownloadersIfNeeded = function(plugin) {
  var enableMenuItems = [];
  for (var i = 0; i < downloaderManager.menuItems.length; i++) {
    var item = downloaderManager.menuItems[i];
    if (item.isSystem) {
      enableMenuItems.push(item);
    } else {
      var downloader = downloaderManager.downloader[item.name];
      if (downloader.checkDownloader()) {
        downloader.updateNPObjectIfNeeded();
        enableMenuItems.push(item);
      } else {
        downloader.resetNPObject();
      }
    }
  }
  return enableMenuItems;
}

downloaderManager.copyLinkToClipboard = function(plugin, url) {
  plugin.CopyToClipboard(url);
}
