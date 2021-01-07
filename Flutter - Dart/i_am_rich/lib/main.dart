

import 'package:flutter/material.dart';

void main() {
  runApp(
    MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('I am Rich'),
          backgroundColor: Colors.blueGrey[900],
        ),
        backgroundColor: Colors.white54 ,
        body: Center(
          child: Image(
            image: AssetImage('images/goz.png'),
          ),
        ),
      ),
    ),
  );
}
