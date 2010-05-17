chrome.extension.onRequest.addListener(
    function(request, sender, sendResponse) {
  sendResponse(true);
});