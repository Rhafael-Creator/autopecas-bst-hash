"""Teste de integracao do executavel: entrada real e saida observada."""
import subprocess
entrada = "abc\n1\n20\nAna Silva\n1\n10\nBruno Souza\n4\n5\n1\nFiltro de oleo\n10\n1850\n9\n20\n1\n3\n6\n1\n0\n"
r = subprocess.run(["./build/autopecas"], input=entrada, text=True, capture_output=True, check=True)
for trecho in ["Informe um inteiro", "Cliente cadastrado.", "Peca cadastrada.", "Venda realizada", "Filtro de oleo | qtd 7 | R$ 18,50"]:
    assert trecho in r.stdout, trecho
assert r.stdout.index("10 | Bruno Souza") < r.stdout.index("20 | Ana Silva")
for entrada in ["", "1\n", "5\n1\n", "9\n1\n", "x" * 200 + "\n0\n"]:
    subprocess.run(["./build/autopecas"], input=entrada, text=True, capture_output=True, check=True)
print("MENU: entrada invalida, nomes com espacos, percurso ordenado, venda e EOF OK")
