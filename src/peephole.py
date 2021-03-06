def match_ret_qret(a,b):
	return a=='ret.0' and b=='qret.0'

def replace_ret_qret(a,b):
	return ['ret.0']

# -----------------------------------------------------------------------------

# TODO: more ops
# TODO: better detection
def match_push_push_alu(a,b,c):
	if not a.startswith('push.'): return False
	if not b.startswith('push.'): return False
	if '@' in a or '@' in b: return False
	op = c.split('.')[0]
	if op not in ['add','sub','shl','shr','and','or','xor','mul','div','mod']: return False
	try:
		va = int(a.split('.')[1])
	except:
		return False
	try:
		vb = int(b.split('.')[1])
	except:
		return False
	return True

def replace_push_push_alu(a,b,c):
	va = int(a.split('.')[1])
	vb = int(b.split('.')[1])
	op = c.split('.')[0]
	if   op=='add': v = va  + vb
	elif op=='sub': v = va  - vb
	elif op=='mul': v = va  * vb
	elif op=='div': v = va // vb
	elif op=='mod': v = va  % vb
	elif op=='shl': v = va << vb
	elif op=='shr': v = va >> vb
	elif op=='and': v = va  & vb
	elif op=='or':  v = va  | vb
	elif op=='xor': v = va  ^ vb
	return [f'push.{v}']

# -----------------------------------------------------------------------------

def match_push_alu(a,b):
	if not a.startswith('push.'): return False
	if b not in ['add.0','mul.0','eq.0','ne.0','lt.0','gt.0','le.0','ge.0']: return False
	return True

def replace_push_alu(a,b):
	va = a.split('.')[1]
	op = b.split('.')[0]
	return [f"{op}i.{va}"]

# -----------------------------------------------------------------------------

def match_add_getset(a,b):
	if a!='add.0': return False
	if b not in ['get.0','set.0']: return False
	return True

def replace_add_getset(a,b):
	op = b.split('.')[0]
	return [f"add{op}.0"]

# -----------------------------------------------------------------------------

def match_rget_add_getset(a,b,c):
	if not a.startswith('rget.'): return False
	if b!='add.0': return False
	if c not in ['get.0','set.0']: return False
	return True

def replace_rget_add_getset(a,b,c):
	arg = a.split('.')[1]
	op = c.split('.')[0]
	return [f"iadd{op}.{arg}"]

# -----------------------------------------------------------------------------

def match_ret_goto(a,b,c):
	return a=='ret.0' and b==']:' and c=='goto.@['

def replace_ret_goto(a,b,c):
	return ["ret.0"]

# -----------------------------------------------------------------------------

rules = [
	(3, match_ret_goto, replace_ret_goto),
	(2, match_ret_qret, replace_ret_qret),
	(3, match_push_push_alu, replace_push_push_alu),
	(2, match_push_alu, replace_push_alu),
	(3, match_rget_add_getset, replace_rget_add_getset),
	(2, match_add_getset, replace_add_getset),
]
