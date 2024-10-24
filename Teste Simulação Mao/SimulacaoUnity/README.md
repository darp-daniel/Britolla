# Configuração do Modelo de Mão no Unity

## 1. Importar o Modelo da Mão
- Importe o modelo da mão: arraste e solte o arquivo do modelo (como `.fbx`, `.obj`, etc.) na pasta **Assets** do Unity.
- O Unity importará o modelo, e você poderá ver o arquivo na pasta **Assets**.

## 2. Adicionar o Modelo à Cena
- Arraste o modelo da mão da pasta **Assets** para a **Hierarquia**. Isso criará um GameObject na cena.
- Renomeie o GameObject para `Mão`.

## 3. Configurar os Dedos como Filhos
- Se o modelo da mão já inclui os dedos, você pode precisar organizar a hierarquia:
  - Expanda o GameObject `Mão` na Hierarquia. Verifique se os dedos estão como filhos desse GameObject. 
  - Se não estiverem:
    - Clique com o botão direito no GameObject da mão e selecione **Create Empty** para cada dedo. Renomeie esses GameObjects como `Dedão`, `Indicador`, etc.
    - Arraste as partes do modelo (ou placeholders, se não estiverem no modelo) para dentro do GameObject correspondente.

## 4. Atribuir Referências no Script HandController
- Com o GameObject `Mão` selecionado, adicione o script `HandController` se ainda não o fez.
- No **Inspector**, você verá os campos para `polegar`, `indicador`, etc. Arraste os GameObjects dos dedos correspondentes (ou as partes do modelo) para esses campos.

## 5. Ajustar as Posições e Rotação dos Dedos
- Posicione cada dedo corretamente em relação ao modelo da mão. Você pode precisar ajustar as posições e rotações diretamente no Inspector para que tudo fique alinhado.
- Teste os movimentos para garantir que eles respondem corretamente aos dados recebidos.

## 6. Teste
- Execute o seu código Python e depois inicie o Unity.
