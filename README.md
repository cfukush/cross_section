# cross_section
卒業研究で用いたプログラムの一部。
CernのROOTと呼ばれる解析ツールを用いたプログラムで、3次元ヒストグラムを生成し、2次元ガウス関数をfittingするプログラムである。

## nuclide_fit.cc
fittingして得られた結果をfitting_results.csvに出力する。

## cross_section.py
fitting_results.csvに出力された値から生成断面積を求める。
誤差にはpropagation of errorを用いている。
csvファイルのdataframe構造は、jupyternoteやgoogle collaboratoryで確認できる。

## nuclide_residual.cc
3次元ヒストグラムから2次元ガウス関数を引いた残差を生成するプログラム。
fittingが上手くいっているのかの確認や、収量の小さな粒子を数え上げる際に、周りの大きな核種を取り除く際に用いる。

