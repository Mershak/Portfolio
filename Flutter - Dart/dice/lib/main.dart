import 'package:flutter/material.dart';
import 'dart:math';

void main() {
  return runApp(
    MaterialApp(
      home: Scaffold(
        backgroundColor: Colors.red,
        appBar: AppBar(
          title: Text('Dicee'),
          backgroundColor: Colors.red,
        ),
        body: DicePage(),
      ),
    ),
  );
}

class DicePage extends StatefulWidget {
  @override
  _DicePageState createState() => _DicePageState();
}

class _DicePageState extends State<DicePage> {
  int leftDiceNumber = 1;
  int rightDiceNumber = 1;
  int diceMax = 6;
void rollDice(){
  leftDiceNumber = Random().nextInt(6)+1;
  rightDiceNumber = Random().nextInt(6)+1;
}
  @override
  Widget build(BuildContext context) {
    return Center(
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Expanded(
              flex: 1,
              child: FlatButton(
                onPressed: (){
                  setState(() {
                    rollDice();
                  });
                },
                child: Image.asset('images/dice$leftDiceNumber.png'),
              )),
          //SizedBox(width: 10.0,),
          Expanded(
            flex: 1,
            child: FlatButton(
              onPressed: (){
                setState(() {
                  rollDice();ü
                });
              },
              child: Image.asset('images/dice$rightDiceNumber.png'),
            ),
          ),
        ],
      ),
    );
  }
}
