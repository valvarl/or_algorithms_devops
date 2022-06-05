# 5. Cell formation problem

## Simulated annealing

Algorithm is implemented according to the [paper](https://ir.nctu.edu.tw/bitstream/11536/9500/1/000253183700003.pdf).

Cost function: <img src="http://www.sciweavers.org/tex2img.php?eq=f%20=%20\frac{n_1^\text{in}}{n_1%20%2b%20n_0^\text{in}}&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit="/>

<table class="table table-bordered table-hover table-condensed">
<thead><tr><th title="Field #1">data</th>
<th title="Field #2">cells</th>
<th title="Field #3">best cost</th>
<th title="Field #4">solution</th>
<th title="Field #5">avg time, s</th>
</tr></thead>
<tbody><tr>
<td>20x20</td>
<td align="right">5</td>
<td align="right">0.428571</td>
<td><a href="test_results/20x20.sol">20x20.sol</a></td>
<td align="right">1.41073</td>
</tr>
<tr>
<td>24x40</td>
<td align="right">11</td>
<td align="right">0.457516</td>
<td><a href="test_results/24x40.sol">24x40.sol</a></td>
<td align="right">37.0192</td>
</tr>
<tr>
<td>30x50</td>
<td align="right">11</td>
<td align="right">0.502513</td>
<td><a href="test_results/30x50.sol">30x50.sol</a></td>
<td align="right">106.986</td>
</tr>
<tr>
<td>30x90</td>
<td align="right">8</td>
<td align="right">0.419753</td>
<td><a href="test_results/30x90.sol">30x90.sol</a></td>
<td align="right">438.931</td>
</tr>
<tr>
<td>37x53</td>
<td align="right">7</td>
<td align="right">0.613146</td>
<td><a href="test_results/37x53.sol">37x53.sol</a></td>
<td align="right">31.9691</td>
</tr>
</tbody></table>
