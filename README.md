# ServoMD_on_BaseBoard
[BaseBoard ver4.0](https://github.com/tk20e/Base-Board-ver4.0-hw)上で動作するサーボモータードライバのfwです。STM32F103C6T6Aが搭載されたBluePillで運用することを想定しています。

## バージョン履歴
|バージョン | バージョン作成日 | 概要 |
| ------- | ------- | ------- |
| 0.9.0 | 2022/12/7 | 初期版 |

## ハードウェア要件
### 機材
- [BaseBoard ver4.0](https://github.com/tk20e/Base-Board-ver4.0-hw.git)
- サーボモータードライバ用ハーネス

### 接続
| サーボモーター | BaseBoard |
| ------- | ------- |
| 5V | 5V(ENC-AB) |
| GND | GND(ENC-AB) |
| PWM | ENC_A(ENC-AB)(TIM1-CH1)<br>ENC_C(ENC-CD)(TIM2-CH1)<br>ENC_E(TIM3-CH1)<br>のいずれか |


以降、MotorA、MotorC、MotorEを次のように定義します。
| モーター名 | PWM |
| ------- | ------- |
| MotorA | ENC_A |
| MotorC | ENC_C |
| MotorE | ENC_E |


上記接続を行うケーブルを「サーボモータードライバ用ハーネス」と呼びます。

### 電流制限について
サーボモーターは比較的省電力なアクチュエータのため、CANバス上の電源を使用して動作させることができます。本来CANバス電源はマイコンなど制御回路を動作させるためのものであり、始動時や、過負荷によるモーターストール時などに大電流を消費すると、周辺の制御回路に悪影響を及ぼします。そこでハーネスの5Vラインに制限抵抗をつなぐことで、制御回路への影響を低減します。具体的な回路と回路定数の計算は以下の通りです。

![circuit.png](circuit.png)

```math
I_{peak} > \fraq{5\,\mathrm{[V]}}{R\,\mathrm{[\ohm]}} \,\text{(ただし$I_{peak}$はBaseBoardに載っているレギュレーターの最大電流)}
```
ただし、複数のサーボモーターを駆動する場合、それぞれのモーターについて右辺を計算し、それらの和がIpeakを超えないようにしてください。

2023年春ロボにて用いられた以下の構成では、Rを以下の値としました。
- モーター台数: 1
- レギュレーター: L78M05(Ipeakを0.6Aと仮定)

```math
R = 9.1 \leq \frec{5}{0.6}
```

なお、抵抗を選定する際は抵抗の消費電力に注意してください。<span style="color: red; font-size: large;">基本的にカーボン抵抗は使用できません。</span>

## ソフトウェア要件
### CAN
CAN通信には標準IDを用いる2.0Aを使用し、ボーレートは1Mbpsです。

また、各ボードはBIDと呼ばれるCANバス上のIDを起点に12個の連続したIDを占有します。それぞれのIDは1つのリソースに対応しており、その対応は以下のようになります。

| ID | 対象モーター | リソース名 | 型 | 説明 |
| -------- | ------- | -------- | ------- | ------- |
| BID | MotorA | cmd | uint8_t | 動作モードを指定します |
| BID+1 | MotorA | target | float | 各モードで解釈が異なります |
| BID+2 | MotorA | diagnostic | byte[8] | 標準出力として使えます |
| BID+3 | MotorA | ack | uint32_t | cmd送出の結果です。0:正常終了 |
| BID+4 | MotorC | cmd | uint8_t | 動作モードを指定します |
| BID+5 | MotorC | target | float | 各モードで解釈が異なります |
| BID+6 | MotorC | diagnostic | byte[8] | 標準出力として使えます |
| BID+7 | MotorC | ack | uint32_t | cmd送出の結果です。0:正常終了 |
| BID+8 | MotorE | cmd | uint8_t | 動作モードを指定します |
| BID+9 | MotorE | target | float | 各モードで解釈が異なります |
| BID+10 | MotorE | diagnostic | byte[8] | 標準出力として使えます |
| BID+11 | MotorE | ack | uint32_t | cmd送出の結果です。0:正常終了 |

以降BID、BID+4、BID+8をそれぞれ、BIDA、BIDC、BIDEと定義します。

modeの種類とそれぞれのmodeに対応するcmdの値およびtargetの意味は以下の通りです。

| MODE | cmdの値 | TARGET |
| ------- | ------- | ------- |
| DEFAULT | 0または1 | 無効 |
| POS | 4 | 目標位置(rad) |