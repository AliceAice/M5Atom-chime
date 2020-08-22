# M5Atom-chime

M5Atom Lite/Matrix からBLEアドバタイズパケットを送信し、  
M5Atom echoにて該当のアドバタイズパケットを検出したら、チャイムを鳴らすサンプルコード。

* m5atom_lite_bcast  
 BLE送信側スケッチ(M5Atom Lite/Matrix用)  
 ボタン押下でBLEにてアドバタイズパケットを送信する

* m5atom_echo_chime  
 BLE受信側スケッチ(M5Atom echo用)  
 scanを行い、一致するデバイス名のアドバタイズパケットを検出したら、スピーカーからチャイム音を鳴らす


## 動作条件
このサンプルコードはArduinoボードマネージャでインストール可能な**esp32ボードの安定版(v1.0.4）では動作しません**  
下記リポジトリの最新版を使用してください。(commit hash:4d98cea にて動作確認済)  
https://github.com/espressif/arduino-esp32

最新版の適用手順の一例を簡単に記載します。
1. ZIP形式で最新版をダウンロード  
[Download ZIP]でリポジトリソースをダウンロード

2. 取得したzipの中身を展開  
ユーザ指定のスケッチブックディレクトリ配下に下記ディレクトリを作成し、その配下にzipの中身を展開する   
　<sketch DIR>\hardware\espressif\esp32\  
　※<sketch DIR>は変更していなければ[C:\Users\PCのユーザ名\Documents\Arduino]  

3. get.exeを実行  
下記パスのget.exeを「管理者として実行」で実行し、必要ファイルをダウンロード(約10分)  
　<sketch DIR>\hardware\espressif\esp32\tools\get.exe  

4. Arduino IDEを再起動してボード設定を変更  
[ツール]⇒[ボード]メニューに「ESP 32 Arduino(in sketchbook)」が追加されているので「M5Stick-C」を選択  


### チャイム音素材について
・チャイム音の音源は「魔王魂」さんのデータを使用させていただきました。  
・音声データの変換には「lang-ship」さんの音声変換ツールを利用させていただきました。  



以上
