var plugin = document.getElementById('pluginId');
var useContextMenuAPI = true;
localStorage['defaultDownloader'] =
    localStorage['defaultDownloader'] || 'chrome_downloader';
localStorage['contextMenu'] = localStorage['contextMenu'] || 'true';
var enableDownloaders = downloaderManager.getEnableDownloader(plugin);

chrome.extension.onRequest.addListener(function(request, sender, response) {
  switch(request.msg) {
  case 'init_loaded':
    response({
      'defaultDownloader': localStorage['defaultDownloader'],
      'contextMenu': localStorage['contextMenu'],
      'useContextMenuAPI': useContextMenuAPI,
      'contextMenuList': enableDownloaders});
    break;
  case 'thunder':
  case 'mini_thunder':
  case 'flashget':
  case 'mini_flashget':
  case 'qq_whirlwind':
  case 'emule':
  case 'orbit':
  case 'idm':
  case 'fdm':
    downloaderManager.downloader(
        request.msg, request.link, plugin, request.pageUrl).download();
    break;
  case 'copyLink':
    downloaderManager.copyLinkToClipboard(plugin, request.link.url);
    break;
  case 'contextMenu':
    localStorage['contextMenu'] = 'false';
    response({contextMenu: false});
    break;
  case 'downloadAll':
    downloaderManager.downloader(
        request.downloader, request.links, plugin, request.pageUrl).
        downloadAll();
    break;
  }
});

chrome.tabs.onSelectionChanged.addListener(function(tabId) {
  updateDownloaders();
  var js = 'chrome.tabs.executeScript(' + tabId + ', {file: "npdownload.js"});';
  chrome.tabs.sendRequest(tabId, {
    'msg': 'init_check',
    'defaultDownloader': localStorage['defaultDownloader'],
    'contextMenu': localStorage['contextMenu'],
    'useContextMenuAPI': useContextMenuAPI,
    'contextMenuList': enableDownloaders
  }, function(response) {
    // If content script is loaded
    if (response)
      js = '';
  });
  setTimeout(function() {
    eval(js);
  }, 500);
});

function updateDownloaders() {
  var downloaders = downloaderManager.getEnableDownloader(plugin);
  console.log(downloaders.length);
  if (downloaders.length == enableDownloaders.length) {
    for (var i = 0; i < downloaders.length; i++)
      if (downloaders[i].name != enableDownloaders[i].name)
        break;
    if (i == downloaders.length)
      return;    
  }
  for (var i = 0; i < downloaders.length; i++)
    if (downloaders[i].name == localStorage['defaultDownloader'])
      break;
  if (i == downloaders.length)
    localStorage['defaultDownloader'] = 'chrome_downloader'; 
  enableDownloaders = downloaders;
  chrome.contextMenus.removeAll();
  createContextMenu(plugin);
}

if (useContextMenuAPI) {
  createContextMenu(plugin);
}

function createContextMenu(plugin) {
  for (var i = 0; i < enableDownloaders.length; i++) {
    var downloader = enableDownloaders[i];
    if (!downloader.isSystem) {
      var title = chrome.i18n.getMessage(downloader.showName);
      var name = downloader.name;
      contextMenuDownload(title, name, plugin);
      if (downloader.supportDownloadAll) {
        title = chrome.i18n.getMessage(downloader.showName2);
        contextMenuDownloadAll(title, name);
      }
    }
  }
}

function contextMenuDownload(title, downloader, plugin) {
  chrome.contextMenus.create({title: title, contexts: ['link'],
      onclick: function(info, tab){
    var link = {};
    link.url = info.linkUrl;
    link.text = info.selectionText || ' ';
    link.pageUrl = info.pageUrl ;
    downloaderManager.downloader(downloader, link, plugin, info.pageUrl).download();
  }}, function() {});
}

function contextMenuDownloadAll(title, downloader) {
  chrome.contextMenus.create({title: title, contexts: ['all'],
      onclick: function(info, tab) {
    if (info.frameUrl) {
      chrome.tabs.executeScript(null,
          {code: 'npDownload.checkFrameByUrlAndDown("' +
          info.frameUrl + '", "' + downloader + '")', allFrames: true})
    } else {
      chrome.tabs.executeScript(null,
          {code: 'npDownload.sendDownloadAllCommandToBg("' +
          downloader + '");'})
    }
  }});
}

