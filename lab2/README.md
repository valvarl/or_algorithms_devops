# 2. 0-1 knapsack problem

* Dynamic programming algorithm
* Approximation algorithms
    * 2-Approximation
    * FPTAS
*  Least Cost B&B algorithm

<table class="table table-bordered table-hover table-condensed">
<thead><tr><th title="Field #1">data</th>
<th title="Field #2">algorithm</th>
<th title="Field #3">price</th>
<th title="Field #4">weight</th>
<th title="Field #5">time per 100k rep., s</th>
<th title="Field #6">interim solutions</th>
</tr></thead>
<tbody><tr>
<td rowspan="4">p01</td>
<td>DP</td>
<td align="right">309</td>
<td align="right">165</td>
<td align="right">4.54887</td>
<td align="right">1650</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">309</td>
<td align="right">165</td>
<td align="right">0.249447</td>
<td align="right">11</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">309</td>
<td align="right">165</td>
<td align="right">4.41283</td>
<td align="right">1660</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">309</td>
<td align="right">165</td>
<td align="right">2.30782</td>
<td align="right">32</td>
</tr>
<tr>
<td rowspan="4">p02</td>
<td>DP</td>
<td align="right">51</td>
<td align="right">26</td>
<td align="right">0.557674</td>
<td align="right">130</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">47</td>
<td align="right">23</td>
<td align="right">0.165083</td>
<td align="right">6</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">51</td>
<td align="right">26</td>
<td align="right">0.605311</td>
<td align="right">135</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">51</td>
<td align="right">26</td>
<td align="right">0.711793</td>
<td align="right">10</td>
</tr>
<tr>
<td rowspan="4">p03</td>
<td>DP</td>
<td align="right">150</td>
<td align="right">190</td>
<td align="right">3.06968</td>
<td align="right">1140</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">146</td>
<td align="right">179</td>
<td align="right">0.18163</td>
<td align="right">7</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">150</td>
<td align="right">190</td>
<td align="right">3.12938</td>
<td align="right">1146</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">150</td>
<td align="right">190</td>
<td align="right">1.56472</td>
<td align="right">24</td>
</tr>
<tr>
<td rowspan="4">p04</td>
<td>DP</td>
<td align="right">107</td>
<td align="right">50</td>
<td align="right">1.23409</td>
<td align="right">350</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">97</td>
<td align="right">45</td>
<td align="right">0.204052</td>
<td align="right">8</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">107</td>
<td align="right">50</td>
<td align="right">1.29945</td>
<td align="right">357</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">107</td>
<td align="right">50</td>
<td align="right">1.45545</td>
<td align="right">24</td>
</tr>
<tr>
<td rowspan="4">p05</td>
<td>DP</td>
<td align="right">900</td>
<td align="right">104</td>
<td align="right">2.68133</td>
<td align="right">832</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">770</td>
<td align="right">65</td>
<td align="right">0.223181</td>
<td align="right">9</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">870</td>
<td align="right">85</td>
<td align="right">2.56241</td>
<td align="right">840</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">900</td>
<td align="right">104</td>
<td align="right">1.63623</td>
<td align="right">21</td>
</tr>
<tr>
<td rowspan="4">p06</td>
<td>DP</td>
<td align="right">1735</td>
<td align="right">169</td>
<td align="right">3.24566</td>
<td align="right">1190</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">1478</td>
<td align="right">140</td>
<td align="right">0.215007</td>
<td align="right">8</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">1735</td>
<td align="right">169</td>
<td align="right">3.3237</td>
<td align="right">1197</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">1735</td>
<td align="right">169</td>
<td align="right">3.32812</td>
<td align="right">58</td>
</tr>
<tr>
<td rowspan="4">p07</td>
<td>DP</td>
<td align="right">1458</td>
<td align="right">749</td>
<td align="right">31.5543</td>
<td align="right">11250</td>
</tr>
<tr>
<td>2-Approx</td>
<td align="right">1441</td>
<td align="right">740</td>
<td align="right">0.519255</td>
<td align="right">16</td>
</tr>
<tr>
<td>FPTAS</td>
<td align="right">1458</td>
<td align="right">749</td>
<td align="right">31.5029</td>
<td align="right">11265</td>
</tr>
<tr>
<td>B&amp;B</td>
<td align="right">1458</td>
<td align="right">749</td>
<td align="right">97.2301</td>
<td align="right">1058</td>
</tr>
</tbody></table>