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

/* Thunder Lite */
function ThunderLite(plugin) {
  ThunderLite.superClass.constructor.apply(this, arguments);
  this.progId = 'MiniThunderAgent.Agent.1';
}
extend(ThunderLite, Downloader);

ThunderLite.prototype.download = function(linkObj) {
  this.npObject.AddTask5(
      linkObj.url, '', '', '', '', -1, 0, -1, '', '', '', 1, '', -1);
  this.npObject.CommitTasks5(1, 1);
}

ThunderLite.prototype.downloadAll = function(links) {
  for (var i = 0; i < links.length; i++) {
    this.npObject.AddTask5(
        links[i].url, '', '', links[i].text, '', -1, 0, -1, '', '', '', 1);
  }
  this.npObject.CommitTasks5(1, 1);
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
function Orbit(plugin) {
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

IDM.prototype.downloadAll = function(links, pageUrl) {
  var urls = [];
  var texts = [];
  for (var i = 0; i < links.length; i++) {
    urls.push(links[i].url);
    texts.push(links[i].text);
  }
  this.npObject.DownloadAll(urls, texts, pageUrl);
}

/* FDM */
function FDM(plugin) {
  FDM.superClass.constructor.apply(this, arguments);
  this.progId = 'WG.WGUrlReceiver';
  this.progId2 = 'WG.WGUrlListReceiver';
}
extend(FDM, Downloader);

FDM.prototype.updateNPObjectIfNeeded = function() {
  // For Free Download Manager, create NPObject before downloading intead of
  // create NPObject here, because unlike other downloaders, when its NPObject
  // is created, its .exe executes at once.
}

FDM.prototype.resetNPObject = function() {
  this.npObject = null;
  this.npObjectAll = null;
}

FDM.prototype.download = function(linkObj) {
  if (!this.npObject)
    this.npObject = this.plugin.CreateObject(this.progId);
  this.npObject.Url = linkObj.url;
  this.npObject.Comment = linkObj.text;
  this.npObject.Referer = linkObj.pageUrl;
  this.npObject.Cookies = "";
  this.npObject.AddDownload();
}

FDM.prototype.downloadAll = function(links, pageUrl) {
  // Create new NPObject every time when downloading all links, 
  // because there is no way to clear the url list from last time.
  this.npObjectAll = this.plugin.CreateObject(this.progId2);
  this.npObjectAll.Referer = pageUrl;
  this.npObjectAll.Cookies = "";
  for (var i = 0; i < links.length; i++) {
    this.npObjectAll.Url = links[i].url;
    this.npObjectAll.Comment = links[i].text;
    this.npObjectAll.AddUrlToList();
  }
  this.npObjectAll.ShowAddUrlListDialog();
}

/* DownloadMaster */
function DownloadMaster(plugin, progId) {
  DownloadMaster.superClass.constructor.apply(this, arguments);
  this.progId = 'dmie.moveurl';
}
extend(DownloadMaster, Downloader);

DownloadMaster.prototype.download = function(linkObj) {
  this.npObject.AddURL(linkObj.url, linkObj.pageUrl);
}

DownloadMaster.prototype.downloadAll = function(links, pageUrl) {
  var urls = [];
  for (var i = 0; l = links.length, i < l; i++) {
    urls.push(links[i].url);
  }
  this.npObject.AddURLs(urls, pageUrl);
}

/* GetGo */
function GetGo(plugin, progId) {
  GetGo.superClass.constructor.apply(this, arguments);
  this.progId = 'URLCatch.IEMenu';
}
extend(GetGo, Downloader);

GetGo.prototype.download = function(linkObj) {
  this.npObject.AddURL(linkObj.url);
}

GetGo.prototype.downloadAll = function(links, pageUrl) {
  this.npObject.AddAllURL(pageUrl);
}

/* DAP */
function DAP(plugin, progId) {
  DAP.superClass.constructor.apply(this, arguments);
  this.progId = 'dapie.catcher';
}
extend(DAP, Downloader);

DAP.prototype.download = function(linkObj) {
  this.npObject.MenuUrl2(linkObj.url, linkObj.pageUrl, '', linkObj.text);
}

/* Linux Downloader */
function LinuxDownloader(plugin, command) {
  LinuxDownloader.superClass.constructor.apply(this, arguments);
  this.command = command;
  this.progId = command.split(' ')[0];
}
extend(LinuxDownloader, Downloader);

LinuxDownloader.prototype.download = function(linkObj) {
  this.npObject.Download(linkObj.url);
}

var downloaderManager = {};

downloaderManager.menuItems = [
  {
    name: 'flashget_windows', showName: 'menu_flashget',
    showName2: 'download_all_with_flashget',
    privateLink: 'flashget://', isLinux: false,
    supportDownloadAll: true, image: 'images/icon_flashget.png'
  }, {
    name: 'mini_flashget_windows', showName: 'menu_mini_flashget',
    showName2: 'download_all_with_mini_flashget',
    privateLink: 'flashget://', isLinux: false,
    supportDownloadAll: true, image: 'images/icon_flashget.png'
  }, {
    name: 'thunder_windows', showName: 'menu_thunder',
    showName2: 'download_all_with_thunder',
    privateLink: 'thunder://', isLinux: false,
    supportDownloadAll: true, image: 'images/icon_thunder.png'
  }, {
    name: 'thunder_lite_windows', showName: 'menu_thunder_lite',
    showName2: 'download_all_with_thunder_lite',
    privateLink: 'thunder://', isLinux: false,
    supportDownloadAll: true, image: 'images/icon_thunderlite.png'
  }, {
    name: 'mini_thunder_windows', showName: 'menu_mini_thunder',
    showName2: 'download_all_with_mini_thunder',
    privateLink: 'thunder://', isLinux: false,
    supportDownloadAll: false, image: 'images/icon_minithunder.png'
  }, {
    name: 'qq_whirlwind_windows', showName: 'menu_qq_whirlwind',
    showName2: 'download_all_with_qq_whirlwind',
    privateLink: '',isLinux: false,
    supportDownloadAll: true, image: 'images/icon_qq.png'
  }, {
    name: 'emule_windows', showName: 'menu_emule',isLinux: false,
    showName2: 'download_all_with_emule', privateLink: 'ed2k://',
    supportDownloadAll: false, image: 'images/icon_emule.png'
  }, {
    name: 'orbit_windows', showName: 'menu_orbit',isLinux: false,
    showName2: 'download_all_with_orbit', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_orbit.png'
  }, {
    name: 'idm_windows', showName: 'menu_idm',isLinux: false,
    showName2: 'download_all_with_idm', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_idm.png'
  }, {
    name: 'fdm_windows', showName: 'menu_fdm', isLinux: false,
    showName2: 'download_all_with_fdm', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_fdm.png'
  }, {
    name: 'download_master_windows', showName: 'menu_download_master',
    showName2: 'download_all_with_download_master', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_downloadmaster.png'
  }, {
    name: 'getgo_windows', showName: 'menu_getgo',
    showName2: 'download_all_with_getgo', privateLink: '',
    supportDownloadAll: true, image: 'images/icon_getgo.png'
  }, {
    name: 'dap_windows', showName: 'menu_dap',
    showName2: 'download_all_with_dap', privateLink: '',
    supportDownloadAll: false, image: 'images/icon_dap.png'
  }, {
    name: 'flashget_linux', showName: 'menu_flashget',
    privateLink: '', isLinux: true, command: 'flashget $URL',
    isUserAdded: false, supportDownloadAll: false,
    image: 'images/icon_flashget_linux.png'
  }, {
    name: 'jdownloader', showName: 'menu_jdownloader',
    privateLink: '', isLinux: true, command: 'jdownloader $URL',
    isUserAdded: false, supportDownloadAll: false,
    image: 'images/icon_jdownloader.png'
  }, {
    name: 'gwget', showName: 'menu_gwget', privateLink: '',
    isLinux: true, command: 'gwget $URL', isUserAdded: false,
    supportDownloadAll: false, image: 'images/icon_gwget.png'
  }, {
    name: 'chrome_downloader', showName: 'menu_chrome',
    isSystem: true, supportDownloadAll: false,
    image: 'images/icon_chrome.png'
  }
];

downloaderManager.downloader = {};

downloaderManager.init = function(plugin) {
  // Create supported downloaders and save them in object
  // downloaderManager.downloader
  downloaderManager.downloader['thunder_windows'] =
      new Thunder(plugin);
  downloaderManager.downloader['mini_thunder_windows'] =
      new MiniThunder(plugin);
  downloaderManager.downloader['thunder_lite_windows'] = 
      new ThunderLite(plugin);
  downloaderManager.downloader['flashget_windows'] =
      new Flashget(plugin);
  downloaderManager.downloader['mini_flashget_windows'] =
      new MiniFlashget(plugin);
  downloaderManager.downloader['qq_whirlwind_windows'] =
      new QQWhirlWind(plugin);
  downloaderManager.downloader['emule_windows'] = new EMule(plugin);
  downloaderManager.downloader['orbit_windows'] = new Orbit(plugin);
  downloaderManager.downloader['idm_windows'] = new IDM(plugin);
  downloaderManager.downloader['fdm_windows'] = new FDM(plugin);
  downloaderManager.downloader['download_master_windows'] =
      new DownloadMaster(plugin);
  downloaderManager.downloader['getgo_windows'] = new GetGo(plugin);
  downloaderManager.downloader['dap_windows'] = new DAP(plugin);

  for (var i = 0; i < downloaderManager.menuItems.length; i++) {
    var item = downloaderManager.menuItems[i];
    if (item.isLinux) {
      // Create downloader in Linux platform and save them.
      downloaderManager.downloader[item.name] =
          new LinuxDownloader(plugin, item.command);
    }
  }

  // Add user added downloaders
  downloaderManager.addCustomDownloaders();
}

downloaderManager.addCustomDownloaders = function() {
  for (var name in localStorage) {
    if (name.indexOf('downloaderConfigure') == 0) {
      var customArr = localStorage[name].split(',');
      downloaderManager.menuItems.push({
        storageName: name, name: customArr[1], showName: 'menu_custom',
        isLinux: true, command: customArr[2], isUserAdded: true,
        supportDownloadAll: false, image: customArr[0]});
      downloaderManager.downloader[customArr[1]] =
          new LinuxDownloader(plugin, customArr[2]);
    }
  }
}

downloaderManager.updateCustomDownloaders = function() {
  for (var i = downloaderManager.menuItems.length - 1; i >= 0; i--) {
    var item = downloaderManager.menuItems[i];
    if (item.isUserAdded)
      downloaderManager.menuItems.splice(i);
  }
  downloaderManager.addCustomDownloaders();
}

downloaderManager.getEnabledDownloaders = function(plugin) {
  var enableMenuItems = [];
  var last = 0;
  for (var i = 0; i < downloaderManager.menuItems.length; i++) {
    var item = downloaderManager.menuItems[i];
    if (item.isSystem) {
      // If the downloader is default downloader of Chrome
      last = i;
    } else {
      var downloader = downloaderManager.downloader[item.name];
      if (downloader.checkDownloader()) {
        if (item.name == "mini_thunder_windows") {
          var thunderLite = 
              downloaderManager.downloader['thunder_lite_windows'];
          if (thunderLite.checkDownloader())
            continue;
        }
        
        // If the downloader is available, update its NPObject
        downloader.updateNPObjectIfNeeded();
        enableMenuItems.push(item);
      } else {
        downloader.resetNPObject();
      }
    }
  }
  enableMenuItems.push(downloaderManager.menuItems[last]);
  return enableMenuItems;
}

downloaderManager.removeDownloader = function(name) {
  downloaderManager.downloader[name].resetNPObject();
  delete downloaderManager.downloader[name];
}

downloaderManager.copyLinkToClipboard = function(plugin, url) {
  plugin.CopyToClipboard(url);
}
