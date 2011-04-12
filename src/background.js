var useContextMenuAPI = true;
var enabledDownloaders = [];
var plugin = document.getElementById('pluginId');

chrome.extension.onRequest.addListener(function(request, sender, response) {
  switch(request.msg) {
  case 'init_loaded':
    response({
      'defaultDownloader': localStorage['defaultDownloader'],
      'contextMenu': localStorage['contextMenu'],
      'useContextMenuAPI': useContextMenuAPI,
      'contextMenuList': enabledDownloaders});
    break;
  case 'thunder_windows':
  case 'thunder_lite_windows':
  case 'mini_thunder_windows':
  case 'flashget_windows':
  case 'mini_flashget_windows':
  case 'qq_whirlwind_windows':
  case 'emule_windows':
  case 'orbit_windows':
  case 'idm_windows':
  case 'fdm_windows':
  case 'download_master_windows':
  case 'getgo_windows':
  case 'dap_windows':
    downloaderManager.downloader[request.msg].download(request.link);
    break;
  case 'copyLink':
    downloaderManager.copyLinkToClipboard(plugin, request.link.url);
    break;
  case 'contextMenu':
    localStorage['contextMenu'] = 'false';
    response({contextMenu: false});
    break;
  case 'downloadAll':
    downloaderManager.downloader[request.downloader].downloadAll(
        request.links, request.pageUrl);
    break;
  default:  // Linux downloaders
    downloaderManager.downloader[request.msg].download(request.link);
    break;
  }
});

chrome.tabs.onSelectionChanged.addListener(function(tabId) {
  updateEnabledDownloaders();
  var js = 'chrome.tabs.executeScript(' + tabId + ', {file: "npdownload.js"});';
  chrome.tabs.sendRequest(tabId, {
    'msg': 'init_check',
    'defaultDownloader': localStorage['defaultDownloader'],
    'contextMenu': localStorage['contextMenu'],
    'useContextMenuAPI': useContextMenuAPI,
    'contextMenuList': enabledDownloaders
  }, function(response) {
    // If content script is loaded
    if (response)
      js = '';
  });
  setTimeout(function() {
    eval(js);
  }, 500);
});

function updateEnabledDownloaders() {
  enabledDownloaders = downloaderManager.getEnabledDownloaders(plugin);
  var numDownloaders = enabledDownloaders.length;
  for (var i = 0; i < numDownloaders; i++)
    if (enabledDownloaders[i].name == localStorage['defaultDownloader'])
      break;
  if (i == numDownloaders)
    localStorage['defaultDownloader'] = 'chrome_downloader';
  if (useContextMenuAPI) {
    chrome.contextMenus.removeAll();
    createContextMenu(plugin);
  }
}

function createContextMenu(plugin) {
  for (var i = 0; i < enabledDownloaders.length; i++) {
    var downloader = enabledDownloaders[i];
    if (!downloader.isSystem) {
      var name = downloader.name;
      var title =
          chrome.i18n.getMessage(downloader.showName).replace(/%s/g, name);
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
      onclick: function(info, tab) {
    var link = {};
    link.url = info.linkUrl;
    link.text = info.selectionText || ' ';
    link.pageUrl = info.pageUrl;
    downloaderManager.downloader[downloader].download(link);
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

function init() {
  // Initialize setting in localStorage
  localStorage['defaultDownloader'] =
      localStorage['defaultDownloader'] || 'chrome_downloader';
  localStorage['contextMenu'] = localStorage['contextMenu'] || 'true';
  localStorage['rawCount'] = localStorage['rawCount'] || 0;

  downloaderManager.init(plugin);

  // Get supported downloaders list
  enabledDownloaders = downloaderManager.getEnabledDownloaders(plugin);
  if (useContextMenuAPI) {
    createContextMenu(plugin);
  }
}

init();
