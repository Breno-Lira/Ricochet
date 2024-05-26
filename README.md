<div align="center">
  
![Captura de tela 2024-05-26 191207](https://github.com/Breno-Lira/Ricochet/assets/142419627/658a73e1-23f2-4acd-b906-099bb2c487ba)

</div>
<div align="center">
  
![image](https://github.com/Breno-Lira/Ricochet/assets/142419627/3ac87268-78e6-4a3e-8030-1f2c20b6046b)

</div>
<p align="center">Ricochet é um jogo inspirado no jogo Breakout do Atari 2600, desenvolvido em C, Ricochet consiste em: o jogador controla uma barra e terá que fazer com que a bola bata nos blocos para conseguir pontos, além de que não pode deixar a bola cair no chão.</p>

---
<br>

# Como Rodar 💻:

1° : Clone o repositório no seu terminal:

```
git clone https://github.com/Breno-Lira/Ricochet
```

2° : Entre na pasta do repositório:

```
cd Ricochet
```

3° : Compile os arquivos:

```
gcc ./src/*.c -I./include -o Ricochet.out
```

4° : Execute o executável:

```
./Ricochet.out
```
<br>

# Como Jogar 🕹️:

- O jogador controlará a barra utilizando as teclas A e D para fazer a barra mover-se para a esquerda e direita respectivamente
- Objetivo do jogador é fazer com que a bola bata em todos os blocos encontrados na tela sem deixar a bola cair no chão
<br>

🟥🟥🟥🟥: blocos vermelhos possuem 3 de vida e terão que ser atingidos 3 vezes para serem destruídos
<br>
<br>
🟧🟧🟧🟧: blocos laranjas possuem 2 de vida e terão que ser atingidos 2 vezes para serem destruídos
<br>
<br>
🟨🟨🟨🟨: blocos amarelos possuem 1 de vida e terão que ser atingidos 1 vez para serem destruídos
<br>
<br>

 - Ao destruir por completo cada bloco, o jogador ganhará 50 pontos
 - Caso deixe a bola cair no chão o jogo se encerrará mostrando na tela o score alcançado
 - Há 3 powerups no jogo: o Freeze, Overdrive e o Clone
<br>

❄️ Freeze: ao pegar esse power up a bolinha ficará 🔵 e quando tocar em algum bloco toda a linha ficará com 1 de vida e na cor azul
<br>
<br>
<div align="center"><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/9dcafc06-322e-4ecc-beda-fe47a846ea1f"></div>

<br>
<br>
💥 Overdrive: ao pegar esse power up a bolinha ficará 🔴 e quebrará todos os blocos com menos de 3 de vida, para os blocos com 3, deixará com apenas 1
<br>
<br>
<div align="center"><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/a0785243-ee35-4764-966a-cae58f5285e3"></div>
<br>
<br>
🧪 Clone: ao pegar esse power up serão geradas mais 2 bolinhas na cor 🟢, terão as mesmas funções da bola branca
<br>
<br>
<br>
<div align="center"><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/250a2387-7fdd-41b9-9422-e8e3da9484a8"></div>
<br>
<br>

  - Ainda existe o modo infinito, onde o jogador não terá uma pontuação máxima para atingir
  - Além disso, o modo consiste em rodadas, onde o jogador só passa para a próxima caso distrua todos os blocos
  - Nesse modo, quanto mais rodadas avançar mais rápida fica a bola

<br>

# Membros 👥:

<div align="center">
  <table border="2">
    <tr>
        <td align="center">Arthur Vinícius</td>
        <td align="center">Breno Lira</td>
        <td align="center">Lucas de Holanda</td>
    </tr>
    <tr>
        <td><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/806fb6bb-cd83-4e3e-a622-d3b741ebc3b2" width="300"></td>
        <td><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/daad86ad-8649-4a47-86fe-bb2bfa87d16f" width="300"></td>
        <td><img src="https://github.com/Breno-Lira/Ricochet/assets/142419627/92f45873-b1a5-4dfb-b338-74e0208a6e57" width="300"></td>
    </tr>
    <tr>
        <td align="center">https://github.com/Art-2005</td>
        <td align="center">https://github.com/Breno-Lira</td>
        <td align="center">https://github.com/LucasHolandaBarros</td>
    </tr>
  </table>
</div>
