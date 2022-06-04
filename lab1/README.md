# Exact pattern matching

* Naive algorithm
  | <img src="https://render.githubusercontent.com/render/math?math=\Theta(m\cdot n)">
* Rabin-Karp algorithm
  | <img src="https://render.githubusercontent.com/render/math?math=\Theta(n)">  in average,
  <img src="https://render.githubusercontent.com/render/math?math=O(m\cdot n)">  at worst
* Knuth–Morris–Pratt algorithm
  | <img src="https://render.githubusercontent.com/render/math?math=\Theta(n)">

<table class="table table-bordered table-hover table-condensed">
<thead><tr><th title="Field #1">data</th>
<th title="Field #2">algorithm</th>
<th title="Field #3">time per 1000 rep., s</th>
<th title="Field #4">letter comparisons</th>
</tr></thead>
<tbody><tr>
<td rowspan="3">bad_1</td>
<td>Naive</td>
<td align="right">0.000551</td>
<td align="right">37</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.000309</td>
<td align="right">5</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.000368</td>
<td align="right">19</td>
</tr>
<tr>
<td rowspan="3">bad_2</td>
<td>Naive</td>
<td align="right">0.015310</td>
<td align="right">1821</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.002041</td>
<td align="right">21</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.003625</td>
<td align="right">289</td>
</tr>
<tr>
<td rowspan="3">bad_3</td>
<td>Naive</td>
<td align="right">1.175314</td>
<td align="right">180201</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.025243</td>
<td align="right">201</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.036906</td>
<td align="right">2899</td>
</tr>
<tr>
<td rowspan="3">bad_4</td>
<td>Naive</td>
<td align="right">54.197993</td>
<td align="right">8002001</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.126754</td>
<td align="right">2001</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.225905</td>
<td align="right">13999</td>
</tr>
<tr>
<td rowspan="3">good_1</td>
<td>Naive</td>
<td align="right">0.116818</td>
<td align="right">2483</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.024784</td>
<td align="right">69</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.027243</td>
<td align="right">1272</td>
</tr>
<tr>
<td rowspan="3">good_2</td>
<td>Naive</td>
<td align="right">0.124748</td>
<td align="right">2727</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.027938</td>
<td align="right">299</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.029548</td>
<td align="right">1511</td>
</tr>
<tr>
<td rowspan="3">good_3</td>
<td>Naive</td>
<td align="right">0.429294</td>
<td align="right">9259</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.120626</td>
<td align="right">1383</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.130179</td>
<td align="right">5312</td>
</tr>
<tr>
<td rowspan="3">good_4</td>
<td>Naive</td>
<td align="right">1.966667</td>
<td align="right">44923</td>
</tr>
<tr>
<td>Rabin-Karp</td>
<td align="right">0.414246</td>
<td align="right">363</td>
</tr>
<tr>
<td>Knuth–Morris–Pratt</td>
<td align="right">0.437015</td>
<td align="right">22628</td>
</tr>
</tbody></table>