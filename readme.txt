-----------
Tab:connect
-----------
can just click each button to connect(this is just for test)
This application will use port 8000, 8500, 9000 if testing on the same machine.

-----------
Tab:search file
-----------
1.to explore a path: 
-select one or both server in the checkbox
-input path
-click explore

2.file search:
-select one or both server in the checkbox
-input path
-input file patterns(each pattern divided by space)
-click search

3.text search
-select one or both server in the checkbox
-input path
-input file patterns(each pattern divided by space)
-input search
-click search

-----------
Tab:upload file
-----------
-select one or both server in the checkbox
-browse file and select a path
-click upload


-----------
Tab:download file
-----------

-select a server in the checkbox
-browse file and select a path
-click download





------------
FEATURES
------------

1:
this application supports:
1.upload;--in upload tab
2.download;--in download tab
3.exploring server directory;--in text search tab
4.file search--in text search tab

2:
performance bonus: 
when processed message, the result text block would show:
1.Response time for server processing
2.Response time for communicate between server and Client 

3:
1.end-point addresses: I used two check boxes to work as two different end-point addresses.
2.support one or more file patterns input
3.text input
4.path input

4
the result block will show:
1.a list of found files
2.machine addresses(here specified as Port number)
3.response time and communicate time

5
passing a path and patterns,the return list is in XML format. (bonus)

6
No option, the explore will always return an XML output

7
every output will show the response time and communicate time in millisecond.(last two lines)
