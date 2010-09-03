
/**************
	FIXME !!!!
	  fragments taken from libgcc.a ( 64bit internal math functions )
***************/

.code16gcc

.global __udivdi3
__udivdi3:
   push   %ebp
   mov    %esp,%ebp
   push   %edi
   push   %esi
   sub    $0x10,%esp
   mov    0x14(%ebp),%eax
   mov    0x8(%ebp),%edx
   mov    0x10(%ebp),%esi
   mov    0xc(%ebp),%edi
  test   %eax,%eax
  mov    %edx,-0x10(%ebp)
  jne    __udivdi3_50
  cmp    %edi,%esi
  ja     __udivdi3_80
  test   %esi,%esi
  jne    __udivdi3_2e
  mov    $0x1,%eax
  xor    %edx,%edx
  div    %esi
  mov    %eax,%esi
__udivdi3_2e:
  mov    -0x10(%ebp),%ecx
  xor    %edx,%edx
  mov    %edi,%eax
  div    %esi
  mov    %eax,%edi
  mov    %ecx,%eax
  div    %esi
  mov    %eax,%ecx
  mov    %edi,%edx
  mov    %ecx,%eax
  add    $0x10,%esp
  pop    %esi
  pop    %edi
  pop    %ebp
  ret
  lea    0x0(%esi),%esi
__udivdi3_50:
  cmp    %edi,%eax
  ja     __udivdi3_70
  bsr    %eax,%edx
  xor    $0x1f,%edx
  mov    %edx,-0xc(%ebp)
  jne    __udivdi3_98
  cmp    -0x10(%ebp),%esi
  jbe    __udivdi3_108
  cmp    %edi,%eax
  jb     __udivdi3_108
__udivdi3_70:
  70:	31 ff                	xor    %edi,%edi
  72:	31 c9                	xor    %ecx,%ecx
__udivdi3_74:
  74:	89 c8                	mov    %ecx,%eax
  76:	89 fa                	mov    %edi,%edx
  78:	83 c4 10             	add    $0x10,%esp
  7b:	5e                   	pop    %esi
  7c:	5f                   	pop    %edi
  7d:	5d                   	pop    %ebp
  7e:	c3                   	ret
  7f:	90                   	nop
__udivdi3_80:
  80:	89 d1                	mov    %edx,%ecx
  82:	89 fa                	mov    %edi,%edx
  84:	89 c8                	mov    %ecx,%eax
  86:	31 ff                	xor    %edi,%edi
  88:	f7 f6                	div    %esi
  8a:	89 c1                	mov    %eax,%ecx
  8c:	89 fa                	mov    %edi,%edx
  8e:	89 c8                	mov    %ecx,%eax
  90:	83 c4 10             	add    $0x10,%esp
  93:	5e                   	pop    %esi
  94:	5f                   	pop    %edi
  95:	5d                   	pop    %ebp
  96:	c3                   	ret
  97:	90                   	nop
__udivdi3_98:
  98:	0f b6 4d f4          	movzbl -0xc(%ebp),%ecx
  9c:	89 f2                	mov    %esi,%edx6a:	0f 82 98 00 00 00
  9e:	d3 e0                	shl    %cl,%eax
  a0:	89 45 ec             	mov    %eax,-0x14(%ebp)
  a3:	b8 20 00 00 00       	mov    $0x20,%eax
  a8:	2b 45 f4             	sub    -0xc(%ebp),%eax
  ab:	89 c1                	mov    %eax,%ecx
  ad:	d3 ea                	shr    %cl,%edx
  af:	0f b6 4d f4          	movzbl -0xc(%ebp),%ecx
  b3:	0b 55 ec             	or     -0x14(%ebp),%edx
  b6:	d3 e6                	shl    %cl,%esi
  b8:	89 c1                	mov    %eax,%ecx
  ba:	89 75 e8             	mov    %esi,-0x18(%ebp)
  bd:	89 fe                	mov    %edi,%esi
  bf:	d3 ee                	shr    %cl,%esi
  c1:	0f b6 4d f4          	movzbl -0xc(%ebp),%ecx
  c5:	89 55 ec             	mov    %edx,-0x14(%ebp)
  c8:	8b 55 f0             	mov    -0x10(%ebp),%edx
  cb:	d3 e7                	shl    %cl,%edi
  cd:	89 c1                	mov    %eax,%ecx
  cf:	d3 ea                	shr    %cl,%edx
  d1:	09 d7                	or     %edx,%edi
  d3:	89 f2                	mov    %esi,%edx
  d5:	89 f8                	mov    %edi,%eax
  d7:	f7 75 ec             	divl   -0x14(%ebp)
  da:	89 d6                	mov    %edx,%esi
  dc:	89 c7                	mov    %eax,%edi
  de:	f7 65 e8             	mull   -0x18(%ebp)
  e1:	39 d6                	cmp    %edx,%esi
  e3:	89 55 ec             	mov    %edx,-0x14(%ebp)
  e6:	72 30                	jb     __udivdi3_118
  e8:	8b 55 f0             	mov    -0x10(%ebp),%edx
  eb:	0f b6 4d f4          	movzbl -0xc(%ebp),%ecx
  ef:	d3 e2                	shl    %cl,%edx
  f1:	39 c2                	cmp    %eax,%edx
  f3:	73 05                	jae    __udivdi3_fa
  f5:	3b 75 ec             	cmp    -0x14(%ebp),%esi
  f8:	74 1e                	je     __udivdi3_118
__udivdi3_fa:
  fa:	89 f9                	mov    %edi,%ecx
  fc:	31 ff                	xor    %edi,%edi
  fe:	e9 71 ff ff ff       	jmp    __udivdi3_74
 103:	90                   	nop
 104:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
__udivdi3_108:
 108:	31 ff                	xor    %edi,%edi
 10a:	b9 01 00 00 00       	mov    $0x1,%ecx
 10f:	e9 60 ff ff ff       	jmp    __udivdi3_74
 114:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
__udivdi3_118:
 118:	8d 4f ff             	lea    -0x1(%edi),%ecx
 11b:	31 ff                	xor    %edi,%edi
 11d:	89 c8                	mov    %ecx,%eax
 11f:	89 fa                	mov    %edi,%edx
 121:	83 c4 10             	add    $0x10,%esp
 124:	5e                   	pop    %esi
 pop    %edi
 pop    %ebp
 ret

