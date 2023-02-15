# Keylogger

Projeto de estudo de funcionamento de malwares (keylogger). Seu funcionamento ocorre a partir de um arquivo em .bat que abre um arquivo 
falso para distrair o usuário enquanto roda um programa malicioso em segundo plano.

- O arquivo .bat serve apenas para inializar o arquivo Keylogger.exe (código do malware compilado e funcional em um arquivo executável) ao mesmo tempo em que 
inicializa outro programa para distrair o usuário (pode ser um jogo, um documento em pdf, qualquer coisa);
- No nosso caso, colocamos o arquivo de um arquivo em .pdf;
- O código do malware foi programado para não abrir nenhuma janela, logo, só é possível finalizar sua execução através do gerenciador de tarefas;
- O malware captura todas as teclas digitadas pelo vítima, além de anotar a aba acessada e o horário de digitação;
- Os dados não são enviados para terceiros ou divulgados. Estes são salvos em um documento na própria máquina.

#ATENÇÃO: O projeto em questão não possui fins maliciosos, nem deve ser usado para tal. Ele possui fins educacionais.
