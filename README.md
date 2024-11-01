
# Linux System V Message Queue Test

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=Arithmos-tools_IPC_test&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=Arithmos-tools_IPC_test)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=Arithmos-tools_IPC_test&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=Arithmos-tools_IPC_test)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=Arithmos-tools_IPC_test&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=Arithmos-tools_IPC_test)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=Arithmos-tools_IPC_test&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=Arithmos-tools_IPC_test)
[![Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=Arithmos-tools_IPC_test&metric=vulnerabilities)](https://sonarcloud.io/summary/new_code?id=Arithmos-tools_IPC_test)

このプロジェクトは、LinuxのSystem Vメッセージキューを使用してプロセス間通信を実現する方法を示しています。

## 概要

Linuxにはプロセス間でデータを交換するためのさまざまなメカニズムがありますが、今回はメッセージキューについてお話しします。メッセージキューは、プロセス間通信に特化してカーネルが管理しているため、TCP/IPよりも圧倒的に高速です。

## システムコール

このプロジェクトで使用する主なシステムコールは以下の通りです：

- `msgget(0x44)`
- `msgsnd(0x45)`
- `msgrcv(0x46)`
- `msgctl(0x47)`

## 使い方

このプロジェクトを使用するには、以下の手順に従ってください。

2. プロジェクトをコンパイルします。
   ```bash
   make 実行
   ```

   このコマンドは、2つのプログラムをコンパイルし、最初に送信プログラムを実行し、2秒待ってから受信プログラムを実行します。

## 注意点

- カーネルのSYSVIPCオプション(Inter Process Communication)が有効になっていることを確認してください。
- 相手のプロセスが情報を読み取るためには信号を送るほうがよいですが、この状況ではターゲットプロセスのIDがわからないため、簡単にデータが来るのを待ちます。
