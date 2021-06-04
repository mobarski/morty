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
	if op not in ['add','sub','shl','shr','and','or','mul','div']: return False
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
	elif op=='shl': v = va << vb
	elif op=='shr': v = va >> vb
	elif op=='and': v = va  & vb
	elif op=='or':  v = va  | vb
	return [f'push.{v}']

# -----------------------------------------------------------------------------

rules = [
	(2, match_ret_qret, replace_ret_qret),
	(3, match_push_push_alu, replace_push_push_alu),
]
