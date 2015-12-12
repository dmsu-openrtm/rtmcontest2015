# Start OpenRTM Tutorial

本書では，OpenRTM Tutorialの使い方を説明します．USBメモリにOpenRTM Tutorialを入れるまでと，それ以降に分けて説明します．

[1. はじめに](#section1)  
[2. USBメモリにOpenRTM Tutorial を実装する](#section2)  
[3. USBメモリに実装したOpenRTM Tutorialを利用する](#section3)

## <a name="section1"> 1. はじめに

以下に，OpenRTM Tutorialの動作環境を示します．ご確認ください．

- OS : Windows 7 / 8.1
- ブラウザ : Internet Explorer (IE)

## <a name="section2"> 2. USBメモリにOpenRTM Tutorial を実装する

本項では，USBメモリにOpenRTM Tutorialを実装する手順について解説します．OpenRTM Tutorial入りのUSBメモリを手に入れた方は読み飛ばしていただいて構いません．

1) GitHubの[tork-a/openrtm_tutorial](https://github.com/tork-a/openrtm_tutorial)へアクセスします．

2) 「Download ZIP」をクリックします．1GB近いファイルとなりますので，回線速度によっては相当の時間をダウンロードに要します．

![iusb_1](image/iusb_1.png)

3) ダウンロードしたファイル（openrtm_tutorial-devel.zip）を右クリックし，「すべて展開(T)」をクリックします（Lhaplus等のアーカイバがインストールされている場合は若干手順が異なりますが，最終的なフォルダ名は必ず一致させてください）．

![iusb_3](image/iusb_3.png)

4) 「圧縮（ZIP形式）フォルダーの展開」のウィンドウが開きますので，特に変更を行わず，「展開(E)」をクリックします．なお，2GB近いファイルとなりますので，展開に多少の時間を要します．

![iusb_4](image/iusb_4.png)

5) 展開中に以下に示す警告が表示される場合があります．いずれも「スキップ(S)」を選択してください．

![iusb_5_1](image/iusb_5_1.png)

![iusb_5_2](image/iusb_5_2.png)

6) 展開後はエクスプローラが開きます．「openrtm_tutorial-devel」というフォルダに展開されたことを確認します．

7)  「openrtm_tutorial-devel」フォルダ内に，「openrt_tutorial-devel」というフォルダが現れますので，クリックした後，右クリックし，「名前の変更(M)」をクリックします．

![iusb_7](image/iusb_7.png)

8) フォルダ名を「openrtm_tutorial」に変更し（「-devel」を削除），「Enter」キーを押して確定します．

![iusb_8](image/iusb_8.png)

9) USBメモリをパソコンに差します．

10) 「スタートボタン」，「コンピューター」の順にクリックします．

![iusb_10](image/iusb_10.png)

11) 該当するリムーバブルディスクに，「openrtm_tutorial」のフォルダをコピーします．

![iusb_11](image/iusb_11.png)

## <a name="section3"> 3. USBメモリに実装したOpenRTM Tutorialを利用する

本項では，USBメモリに実装したOpenRTM Tutorialを利用する手順について解説します．途中，いくつかの制約条件が発生しますのでご注意ください．

1) USBメモリをパソコンに差します．

2) 「スタートボタン」，「コンピューター」の順にクリックします．

![uusb_2](image/uusb_2.png)

3) 「リムーバブル ディスク (F:)」を開きます（ダブルクリックします）．なお，現在のところ（2015年12月11日），OpenRTM Tutorialを利用するためには，USBメモリのドライブ名が「F」である必要があります．ドライブ名変更手順については，[こちら](02_Change_DriveName.md)を参照してください．

![uusb_3](image/uusb_3.png)

4) 「openrtm_tutorial」のフォルダを開きます（ダブルクリックします）．

![uusb_4](image/uusb_4.png)

5) 「index.htm」を右クリックし，「プログラムから開く」を選択し，「Internet Explorer」をクリックします．

![uusb_5](image/uusb_5.png)

6) 言語選択画面が現れますので，「日本語」，「English」のいずれかをクリックします．以下は「日本語」を選択した場合を例に説明します．

![uusb_6](image/uusb_6.png)

7) デモ選択画面が現れますので，体験したいデモを選択します．なお，ブラウザ下方に，「このWebページはスクリプトやActiveXコントロールを実行しないように制限されています。」と表示されますので，「ブロックされているコンテンツを許可(A)」をクリックします．

![uusb_7](image/uusb_7.png)

8) チュートリアルに従って，デモを進めてください．なお，チュートリアル実行中に以下に示す2つの警告が出ますが，いずれも承諾してください（ActiveXに関する警告，およびファイアウォールに関する警告）．

![uusb_8_1](image/uusb_8_1.png)

![uusb_8_2](image/uusb_8_2.png)