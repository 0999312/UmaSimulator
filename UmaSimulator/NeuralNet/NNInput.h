#pragma once
const int LATEST_NNINPUT_VERSION = 1;
const int NNINPUT_CHANNELS_V1 = 903;
const int NNOUTPUT_CHANNELS_POLICY_V1 = (5 + 3) + 3 + 1 + 6;//5+3�����ѵ������Ϣ���������3��Ů���¼�ѡ��ɫ��1�ǿ�Ů��6��6�������ÿ�β�һ��ȫ����Ч
const int NNOUTPUT_CHANNELS_VALUE_V1 = 2;//��һ��ͨ��������ĸ��ʣ��ڶ���ͨ������Ԥ�ڷ�����Ŀ�����֮��
const int NNOUTPUT_CHANNELS_V1 = NNOUTPUT_CHANNELS_POLICY_V1 + NNOUTPUT_CHANNELS_VALUE_V1;