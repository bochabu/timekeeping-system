import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
// import 'package:googleapis/sheets/v4.dart' as sheets;
import 'package:googleapis_auth/auth_io.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Google Sheets Data',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
      debugShowCheckedModeBanner: false, 
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  List<List<dynamic>> mainTabData = [];
  List<List<dynamic>> attendanceLogData = [];
  List<List<dynamic>> passwordTabData = [];

  @override
  void initState() {
    super.initState();
    fetchSheetData();
  }

  Future<void> fetchSheetData() async {
    final credentials = ServiceAccountCredentials.fromJson(r'''
    {
      
    }
    ''');

    final scopes = [sheets.SheetsApi.spreadsheetsScope];
    final client = await clientViaServiceAccount(credentials, scopes);
    final sheetsApi = sheets.SheetsApi(client);

    const spreadsheetId = '';
    const mainTabRange = 'main tab!A1:F';
    const attendanceLogRange = 'attendance log!A1:E';
    const passwordTabRange = 'password tab!A1:A2';

    final mainTabResponse = await sheetsApi.spreadsheets.values.get(spreadsheetId, mainTabRange);
    final attendanceLogResponse = await sheetsApi.spreadsheets.values.get(spreadsheetId, attendanceLogRange);
    final passwordTabResponse = await sheetsApi.spreadsheets.values.get(spreadsheetId, passwordTabRange);

    setState(() {
      mainTabData = mainTabResponse.values ?? [];
      attendanceLogData = attendanceLogResponse.values ?? [];
      passwordTabData = passwordTabResponse.values ?? [];
    });
  }

  Future<void> _refreshData() async {
    await fetchSheetData();
  }

  List<DataRow> _buildRows(List<List<dynamic>> data) {
    int columnCount = data.isNotEmpty ? data[0].length : 0;
    return data.skip(1).map((row) {
      if (row.length < columnCount) {
        row.addAll(List.filled(columnCount - row.length, ''));
      }
      return DataRow(cells: row.map((cell) => DataCell(Text(cell.toString()))).toList());
    }).toList();
  }

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 3,
      child: Scaffold(
        appBar: AppBar(
          title: Text(
            'Google Sheets Data',
            style: TextStyle(color: Colors.white), 
          ),
          backgroundColor: Colors.blue, 
          actions: [
            IconButton(
              icon: Icon(Icons.refresh, color: Colors.white), 
              onPressed: _refreshData,
            ),
          ],
          bottom: TabBar(
            tabs: [
              Tab(text: 'Main Tab'),
              Tab(text: 'Attendance Log'),
              Tab(text: 'Password Tab'),
            ],
            labelColor: Colors.white, 
            indicatorColor: Colors.white, 
          ),
        ),
        body: TabBarView(
          children: [
            _buildTabContent(mainTabData),
            _buildTabContent(attendanceLogData),
            _buildTabContent(passwordTabData),
          ],
        ),
      ),
    );
  }

  Widget _buildTabContent(List<List<dynamic>> data) {
    if (data.isEmpty) {
      return Center(child: CircularProgressIndicator());
    }
    return ListView(
      children: [
        SingleChildScrollView(
          scrollDirection: Axis.horizontal, 
          child: SingleChildScrollView(
            padding: EdgeInsets.all(16.0),
            child: Container(
              decoration: BoxDecoration(
                color: Colors.white,
                borderRadius: BorderRadius.circular(8.0),
                boxShadow: [
                  BoxShadow(
                    color: Colors.grey.withOpacity(0.5),
                    spreadRadius: 5,
                    blurRadius: 7,
                    offset: Offset(0, 3), 
                  ),
                ],
              ),
              child: DataTable(
                columns: data[0]
                    .map((header) => DataColumn(label: Text(header.toString())))
                    .toList(),
                rows: _buildRows(data),
              ),
            ),
          ),
        ),
      ],
    );
  }
}
