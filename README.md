# Vigenere Cipher para 95 caracteres ASCII - ' ' até '~'

Programa em C++ para cifrar, decifrar e atacar textos usando a cifra de Vigenère.

## Compilação

```bash
g++ vigenere.cpp -o vigenere
```

## Uso

```bash
./vigenere <input_file_path> <encrypt|decrypt|attack> <key>
```

Exemplo para cifrar:

```bash
./vigenere teste.txt encrypt LEMON
```

Exemplo para decifrar:

```bash
./vigenere teste_encrypted.txt decrypt LEMON
```

Exemplo para atacar:

```bash
./vigenere teste_encrypted.txt attack
```

## Parâmetros

- `input_file_path`: caminho do arquivo `.txt` de entrada.
- `encrypt|decrypt|attack`: define se o programa vai cifrar, decifrar ou atacar.
- `key`: chave alfabética usada na cifra de Vigenère.

## Saída

Arquivos gerados:

- ao cifrar: `<nome>_encrypted.txt`
- ao decifrar: `<nome>_decrypted.txt`

Exemplo:

- `teste.txt` -> `teste_encrypted.txt`
- `teste_encrypted.txt` -> `teste_encrypted_decrypted.txt`
