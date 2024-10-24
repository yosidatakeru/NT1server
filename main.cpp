//�T�[�o�[
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <winsock2.h> /* WinSock�̃w�b�_�t�@�C�� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment ( lib, "WSock32.lib" ) /* WinSock���C�u�����̎w�� */

void ChatServer(void)
{


	SOCKET listen_s;
	SOCKET s;
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	int fromlen;
	u_short uport;

	/* �|�[�g�ԍ��̓��� */
	printf("�g�p����|�[�g�ԍ�--> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* ���X���\�P�b�g���I�[�v�� */
	listen_s = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_s == INVALID_SOCKET)
	{
		printf("���X���\�P�b�g�I�[�v���G���[");
		WSACleanup();
		return;
	}
	printf("���X���\�P�b�g���I�[�v�����܂���\n");

	/*�@���X���C�h���Q�l��SOCKADDR_IN�^�ϐ��usaddr�v�̐ݒ菈���@*/
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(listen_s, (const sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		printf("bind�G���[");
		closesocket(listen_s);
		return;
	}

	printf("bind����");

	if (listen(listen_s, 0) == SOCKET_ERROR)
	{
		printf("listen�G���[\n");
		closesocket(listen_s);
		return;
	}

	printf("listen����\n");

	/* �ڑ���ҋ@���� */
	printf("accept�őҋ@���܂�\n");

	fromlen = (int)sizeof(from);

	s = accept(listen_s, (struct sockaddr*)&from, &fromlen); /*�@��SOCKADDR_IN�^�ϐ��ufrom�v���g����accept�֐������s�@*/

	if (s == INVALID_SOCKET) {
		printf("accept�G���[\n");
		closesocket(listen_s);
		return;
	}

	printf("%s���ڑ����Ă��܂���\n", inet_ntoa(from.sin_addr));
	printf("accepet�֐�����\n");

	/* ���X���p�̃\�P�b�g�͂����s�v */
	closesocket(listen_s);

	/* ��b�J�n */
	printf("��b�J�n\n");

	while (1) {

		/*�@���X���C�h���Q�l�ɑ���M��������́@*/
		int nRcv;
		char szBuf[1024];
		printf("���M-->");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(s, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		printf("��M-->%s\n", szBuf);

	}

	/* �\�P�b�g����� */
	closesocket(s);
}


void ChatClient()
{
	SOCKET s;
	SOCKADDR_IN saddr;
	u_short uport;
	char szServer[1024] = { 0 };
	HOSTENT* lpHost;
	unsigned int addr;

	/* �|�[�g�ԍ��̓��� */
	printf("�g�p����|�[�g�ԍ� --> ");
	scanf_s("%hd", &uport);
	fflush(stdin);

	/* �T�[�o���܂��̓T�[�o��IP�A�h���X����� */
	printf("IP�A�h���X -->");
	scanf_s("%s", szServer, 1024);
	fflush(stdin);

	/* �\�P�b�g���I�[�v�� */

	s = socket(AF_INET, SOCK_STREAM, 0); /*�@��socket�֐��@*/

	if (s == INVALID_SOCKET) {
		printf("�\�P�b�g�I�[�v���G���[\n");
		return;
	}

	/* �T�[�o�[�𖼑O�Ŏ擾���� */
	lpHost = gethostbyname(szServer);

	if (lpHost == NULL) {
		/* �T�[�o�[��IP�A�h���X�Ŏ擾���� */
		addr = inet_addr(szServer);
		lpHost = gethostbyaddr((char*)&addr, 4, AF_INET);
	}

	if (lpHost == NULL) {
		printf("Host��������܂���\n");
		closesocket(s);
		return;
	}

	/*�@���X���C�h���Q�l��SOCKADDR_IN�^�ϐ��usaddr�v�̐ݒ菈���@*/

	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)lpHost->h_addr);


	if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
		closesocket(s);
		return;
	}

	printf("�T�[�o�[�ɐڑ��ł��܂���\n");
	while (1) {

		/*�@�X���C�h���Q�l�ɑ���M��������́@*/
		int nRcv;
		char szBuf[1024];
		printf("���M-->");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(s, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		printf("��M-->%s\n", szBuf);
	}

	/* �\�P�b�g����� */
	closesocket(s);
}

int main(void)
{
	WSADATA wsaData;
	int mode;

	/* WinSock������ */
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		/* �G���[ */
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return 1;
	}

	/* �T�[�o�[���H�N���C�A���g���H */
	printf("�T�[�o�[�Ȃ�0����� �N���C�A���g�Ȃ�1����� --> ");

	scanf_s("%d", &mode);

	fflush(stdin);

	if (mode == 0) {
		/* �T�[�o�[�Ƃ��ċN�� */
		ChatServer();
	}
	else {
		/* �N���C�A���g�Ƃ��ċN�� */
		ChatClient();
	}

	/* WinSock�̏I������ */
	WSACleanup();

	return 0;
}


