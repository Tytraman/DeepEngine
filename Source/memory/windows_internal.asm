.code

getTEB proc EXPORT
	mov rax, qword ptr gs:[30h]
	ret
getTEB endp

end