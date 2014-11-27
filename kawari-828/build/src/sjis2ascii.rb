#!/bin/env ruby

# �\�[�X�R�[�h����ShiftJIS�������ASCII�փG���R�[�h����

require "kconv"

while ($<.gets)
	line=Kconv::tosjis($_)

	kanji1=0
	line.each_byte do |c|
		if kanji1==0
			if (((c^0x20)-0xa1)&0xff)<=0x3b
				# SJIS
				# 0x00-0x7f ASCII
				# 0x80-0x9f,0xe0-0xfc ������S�p1�o�C�g��
				# 0xa0-0xdf �����锼�p�J�i
				# ���Ȃ݂�2�o�C�g�ڂ�0x40-0xfc
				kanji1=c
			else
				putc c
			end
		else
#			printf "\\x%x\\x%x",kanji1,c
			printf "\\%03o\\%03o",kanji1,c
			kanji1=0
		end
	end

end

