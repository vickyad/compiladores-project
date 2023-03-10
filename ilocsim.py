#!/usr/bin/env python3

import sys
import re
import argparse

def eprint(*s):
   print(*s,file=sys.stdout)

class Operation:
   def __init__(self, s, patterns=[]):
      if not patterns:
         self.build_patterns(patterns)
      for p in patterns:
         m = re.match(p, s)
         if m: break
      self.op = None if m is None else [int(x) if re.match('^(-?[0-9]+)$',x) else x for x in m.groups()]

   def pattern(self,ops,operandsLeft,operandsRight,arrow='=>'):
      if type(ops) in (list,set):
         ops = '(' + ('|'.join(['(?:%s)'%(x) for x in ops])) + ')'
      else:
         ops = '('+ops+')'
      if type(operandsLeft) in (list,set):
         operandsLeft = r'\s*,\s*'.join(operandsLeft)
      if type(operandsRight) in (list,set):
         operandsRight = r'\s*,\s*'.join(operandsRight)
      if not operandsLeft and not operandsRight:
         return ops
      arrow = '[=-]>' # accept any arrow
      arrow = r'\s*'+arrow+r'\s*'
      return (
         r'^\s*' +
         ops +
         r'\s+' +
         operandsLeft +
         arrow +
         operandsRight +
         r'\s*$')

   def build_patterns(self, patterns):
      register = r'([Rr]\w+)'
      const    = r'((?:-?[0-9]+)|(?:@\w+))'
      label    = r'([Ll]\w+)' # do labels really MUST start with L?
      l = []
      # op
      l.append(('nop',None,None))
      # halt
      l.append(('halt',None,None))
      # op => l
      l.append(('jumpI','',label,'->'))
      # op => r
      l.append(('jump','',register,'->'))
      # op c => r
      l.append(('loadI',const,register))
      # op r => r
      ops = ['load','cload','store','cstore','i2i','c2c','c2i','i2c']
      l.append((ops, register, register))
      # op r => r c
      ops = ['storeAI','cstoreAI']
      l.append((ops,register,[register,const]))
      # op r => l l
      l.append(('cbr',register,[label,label],'->'))
      # op r => r r
      ops = ['storeAO','cstoreAO']
      l.append((ops,register,[register,register]))
      # op r, c => r
      ops = ['addI','subI','rsubI','multI','divI','rdivI','lshiftI','rshiftI',
      'andI','orI','xorI','loadAI','cloadAI']
      l.append((ops, [register,const], register) )
      # op r, r => r
      ops = ['add','sub','mult','div','lshift','rshift','and','or','xor',
      'loadAO','cloadAO']
      l.append((ops, [register,register], register))
      ops = ['cmp_LT','cmp_LE','cmp_EQ','cmp_GE','cmp_GT','cmp_NE']
      l.append((ops, [register,register], register,'->'))

      [patterns.append(re.compile(self.pattern(*x))) for x in l]

#
# Classe que guarda os valores
#
class Storage:
   def __init__(self, values=None, default_f=lambda: 0xdeadbeef):
      self.rfpFirstUpdate = 0
      self.rspFirstUpdate = 0
      self.stats = {'read':0,'write':0}
      self.value = {}
      if values:
         for x in values:
            self.value[x] = values[x]
      self.default_f = default_f
      self.default_f = lambda: 0
   def get(self,x):
      return self.value[x] if x in self.value else self.default_f()
   def __str__(self):
      return self.value.__str__()
   def __len__(self):
      return self.value.__len__()
   def __iter__(self):
      return self.value.__iter__()
   def __getitem__(self, x):
      self.stats['read'] += 1
      return self.get(x)
   def __setitem__(self, x, y):
      update = 1
      if (strict):
         if (x == "rbss"):
            update = 0
         if (x == "rfp" and self.rfpFirstUpdate == 0):
            update = 0
            self.rfpFirstUpdate = 1
         if (x == "rsp" and self.rspFirstUpdate == 0):
            update = 0
            self.rspFirstUpdate = 1

      if (update):
         self.stats['write'] += 1
         self.value[x] = y
      else:
         eprint("Change of", x, "ignored (strict mode)");

#
# Classe do Simulador ILOC
#
class Sim:
   def __init__(self, program, bss, fp, trace=False):
      self.reg = Storage({'rbss':bss, 'rfp':fp, 'rpc':0})
      self.mem = Storage()
      self.labels = {}
      self.ip = 0
      self.stats = {'instructions':0}
      self.varmap = {}
      self.trace = trace
      self.code = self.program_load(program)

   def show_stats(self):
      l = [x for x in self.stats if x != 'instructions']
      l.sort(key=lambda x:-self.stats[x])
      eprint('count\tinstruction')
      for x in l:
         eprint('%5d\t%s'%(self.stats[x],x))
      eprint('\ninstructions executed: %4d'%(self.stats['instructions']))
      eprint('register         read: %4d write: %4d'%(self.reg.stats['read'],self.reg.stats['write']))
      eprint('memory           read: %4d write: %4d'%(self.mem.stats['read'],self.mem.stats['write']))
      eprint('\n%10s %8s'%('register','value'))
      l = [x for x in self.reg]
      l.sort()
      for x in l:
         eprint('%10s %8d'%(x,self.reg.get(x)))
      eprint('\n%10s %8s'%('memory','value'))
      l = [x for x in self.mem]
      l.sort()
      for x in l:
         eprint('%.8d %8d'%(x,self.mem.get(x)))

   def show_instructions_executed_counting(self):
      l = [x for x in self.stats if x != 'instructions']
      l.sort(key=lambda x:-self.stats[x])
      for x in l:
         eprint('%d %s'%(self.stats[x],x))

   def show_memory(self):
      l = [x for x in self.mem]
      l.sort()
      for x in l:
         eprint('%.8d %d'%(x,self.mem.get(x)))


   def map_vars(self, code, varmap=None):
      if type(code) not in (list,set):
         code = [code]
      if not varmap:
         varmap = {}
      n = max([varmap[x] for x in varmap]) if varmap else 0
      ops1 = set(('loadI',))
      ops2 = set(('addI','subI','rsubI','multI','divI','rdivI','lshiftI','rshiftI',
                  'andI','orI','xorI','loadAI','cloadAI'))
      ops3 = set(('storeAI','cstoreAI'))
      ops = set().union(ops1,ops2,ops3)
      pos = dict()
      for x in ops1: pos[x] = 1
      for x in ops2: pos[x] = 2
      for x in ops3: pos[x] = 3
      for ip in range(len(code)):
         op = code[ip]
         if op[0] in ops:
            p = pos[op[0]]
            if type(op[p]) != str or re.match('^([0-9]+)$',op[p]): continue
            if op[p] not in varmap:
               varmap[op[p]] = n
               n += 1
            op[p] = varmap[op[p]]

   def map_labels(self, code, labels):
      for op in code:
         if 'cbr' == op[0]:
            op[2] = labels[op[2]]
            op[3] = labels[op[3]]
         elif 'jumpI' == op[0]:
            op[1] = labels[op[1]]

   def program_load(self, program):
      varmap = {}
      labelmap = {}
      l = []
      for linha, s in enumerate(program.split('\n')):
         if '//' in s:
            s = s[:s.find('//')]
         s = s.replace('\t',' ').replace('\r','').strip(' ')
#         if s:
#            eprint(len(l),s)
         while 1:
            # Procura por labels
            m = re.match(r'^\s*(\w+)\s*:.*',s)
            if not m: break
            s = s[s.find(':')+1:].strip(' ')
            self.labels[m.group(1)] = len(l)
         if not s: continue
        #  print(s)
         o = Operation(s).op
         if o is None:
             print("Erro sintático na linha", linha+1, "com a instrução (", s, ")")
             sys.exit(1)
         else:
             l.append( Operation(s).op )
      self.map_labels(l,self.labels)
      self.map_vars(l)
#      eprint('---')
#      for i in range(len(l)):
#         eprint(i,l[i])
#      eprint('---')
      return l
   def run(self):
      while len(self.code) > self.ip:
         self.step()
   def step(self):
      if self.trace:
         eprint('%d %s'%(self.ip, self.code[self.ip]))
#         eprint(self.reg['r0'],self.reg['r1'],self.reg['r2'],self.reg['r3'],self.reg['r4'])
      self.stats['instructions'] += 1
      op = self.code[self.ip]
      self.reg['rpc'] = self.ip;
      self.ip += 1
#      print(self.ip-1,op,self.reg.get('r1'),self.reg.get('r2'),self.reg.get('r3'),self.reg.get('r4'))
      if op[0] not in self.stats:
         self.stats[op[0]] = 0
      self.stats[op[0]] += 1
      Sim.__dict__[ 'op_' + op[0] ](self, op[1:])
#      eprint('r',self.reg['r0'],self.reg['r1'],self.reg['r2'],self.reg['r3'],self.reg['r4'])
#      eprint('m',self.mem[0x100001],self.mem[0x100003])

   def op_nop    (self,op): pass
   def op_halt   (self,op): self.ip = len(self.code)+1

   def op_add    (self,op): self.reg[op[2]] = self.reg[op[0]]  + self.reg[op[1]]
   def op_sub    (self,op): self.reg[op[2]] = self.reg[op[0]]  - self.reg[op[1]]
   def op_mult   (self,op): self.reg[op[2]] = self.reg[op[0]]  * self.reg[op[1]]
   def op_div    (self,op): self.reg[op[2]] = self.reg[op[0]] // self.reg[op[1]]
   def op_lshift (self,op): self.reg[op[2]] = self.reg[op[0]] << self.reg[op[1]]
   def op_rshift (self,op): self.reg[op[2]] = self.reg[op[0]] >> self.reg[op[1]]
   def op_and    (self,op): self.reg[op[2]] = self.reg[op[0]]  & self.reg[op[1]]
   def op_or     (self,op): self.reg[op[2]] = self.reg[op[0]]  | self.reg[op[1]]
   def op_xor    (self,op): self.reg[op[2]] = self.reg[op[0]]  ^ self.reg[op[1]]
   def op_cmp_LT (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] <  self.reg[op[1]]) else 0
   def op_cmp_LE (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] <= self.reg[op[1]]) else 0
   def op_cmp_EQ (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] == self.reg[op[1]]) else 0
   def op_cmp_GE (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] >= self.reg[op[1]]) else 0
   def op_cmp_GT (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] >  self.reg[op[1]]) else 0
   def op_cmp_NE (self,op): self.reg[op[2]] = 1 if (self.reg[op[0]] != self.reg[op[1]]) else 0

   def op_addI   (self,op): self.reg[op[2]] = self.reg[op[0]]  + op[1]
   def op_subI   (self,op): self.reg[op[2]] = self.reg[op[0]]  - op[1]
   def op_rsubI  (self,op): self.reg[op[2]] = op[1] - self.reg[op[0]]
   def op_multI  (self,op): self.reg[op[2]] = self.reg[op[0]]  * op[1]
   def op_divI   (self,op): self.reg[op[2]] = self.reg[op[0]] // op[1]
   def op_rdivI  (self,op): self.reg[op[2]] = op[1] // self.reg[op[0]]
   def op_lshiftI(self,op): self.reg[op[2]] = self.reg[op[0]] << op[1]
   def op_rshiftI(self,op): self.reg[op[2]] = self.reg[op[0]] >> op[1]
   def op_andI   (self,op): self.reg[op[2]] = self.reg[op[0]]  & op[1]
   def op_orI    (self,op): self.reg[op[2]] = self.reg[op[0]]  | op[1]
   def op_xorI   (self,op): self.reg[op[2]] = self.reg[op[0]]  ^ op[1]

   def op_load   (self,op): self.reg[op[1]] = self.mem[self.reg[op[0]]]
   def op_loadI  (self,op): self.reg[op[1]] = op[0] #if (op[1] != "rbss") else self.reg[op[1]]
   def op_loadAI (self,op): self.reg[op[2]] = self.mem[self.reg[op[0]]+op[1]]
   def op_loadAO (self,op): self.reg[op[2]] = self.mem[self.reg[op[0]]+self.reg[op[1]]]
#   def op_cload
#   def op_cloadAI
#   def op_cloadAO

   def op_store  (self,op): self.mem[self.reg[op[1]]                ] = self.reg[op[0]]
   def op_storeAI(self,op): self.mem[self.reg[op[1]]+op[2]          ] = self.reg[op[0]]
   def op_storeAO(self,op): self.mem[self.reg[op[1]]+self.reg[op[2]]] = self.reg[op[0]]
#   def op_cload
#   def op_cloadAI

   def op_i2i    (self,op): self.reg[op[1]] = self.reg[op[0]]
#   def op_c2c
#   def op_c2i
#   def op_i2c

   def op_jump   (self,op): self.ip = self.reg[op[0]]
   def op_jumpI  (self,op): self.ip = op[0]
   def op_cbr    (self,op): self.ip = op[1] if self.reg[op[0]] else op[2]

   def op_print  (self,op): print(self.reg[op[0]])
   def op_printI (self,op): print(op[0])

   def test(self, code, beforeReg=None, afterReg=None, beforeMem=None, afterMem=None):
      if not beforeReg: beforeReg = {}
      if not afterReg: afterReg = {}
      if not beforeMem: beforeMem = {}
      if not afterMem: afterMem = {}
      s = Sim(code)
      if 'rbss' not in beforeReg:
         beforeReg['rbss'] = self.reg.get('rbss')
      if 'rbss' not in afterReg:
         afterReg['rbss'] = self.reg.get('rbss')
      s.mem = Storage(beforeMem)
      s.reg = Storage(beforeReg)
      s.step()
      assert(len(s.reg)==len(afterReg))
      print(s.reg)
      print(afterReg)
      for x in s.reg:
         assert(s.reg[x] == afterReg[x])
      assert(len(s.mem)==len(afterMem))
      print(s.mem)
      print(afterMem)
      for x in s.mem:
         assert(s.mem[x] == afterMem[x])
   def test_all(self):
      rbss = self.reg.get('rbss')
      tests =[
      ('nop',),
      ('halt',),
      ('add r1, r2 => r3',{'r1':1,'r2':3},{'r1':1,'r2':3,'r3':4}),
      ('sub r1, r2 => r3',{'r1':1,'r2':3},{'r1':1,'r2':3,'r3':-2}),
      ('mult r1, r2 => r3',{'r1':1,'r2':3},{'r1':1,'r2':3,'r3':3}),
      ('div r1, r2 => r3',{'r1':7,'r2':2},{'r1':7,'r2':2,'r3':3}),
      ('lshift r1, r2 => r3',{'r1':8,'r2':1},{'r1':8,'r2':1,'r3':16}),
      ('rshift r1, r2 => r3',{'r1':8,'r2':1},{'r1':8,'r2':1,'r3':4}),
      ('and r1, r2 => r3',{'r1':1,'r2':1},{'r1':1,'r2':1,'r3':1}),
      ('and r1, r2 => r3',{'r1':1,'r2':0},{'r1':1,'r2':0,'r3':0}),
      ('or r1, r2 => r3',{'r1':0,'r2':1},{'r1':0,'r2':1,'r3':1}),
      ('or r1, r2 => r3',{'r1':0,'r2':0},{'r1':0,'r2':0,'r3':0}),
      ('xor r1, r2 => r3',{'r1':0,'r2':1},{'r1':0,'r2':1,'r3':1}),
      ('xor r1, r2 => r3',{'r1':0,'r2':0},{'r1':0,'r2':0,'r3':0}),
      ('cmp_LT r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':1}),
      ('cmp_LE r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':1}),
      ('cmp_EQ r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':0}),
      ('cmp_GE r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':0}),
      ('cmp_GT r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':0}),
      ('cmp_NE r1, r2 => r3',{'r1':1,'r2':2},{'r1':1,'r2':2,'r3':1}),
      ('addI r1, 7 => r2',{'r1':1},{'r1':1,'r2':8}),
      ('subI r1, 7 => r2',{'r1':1},{'r1':1,'r2':-6}),
      ('rsubI r1, 7 => r2',{'r1':1},{'r1':1,'r2':6}),
      ('multI r1, 7 => r2',{'r1':2},{'r1':2,'r2':14}),
      ('divI r1, 7 => r2',{'r1':22},{'r1':22,'r2':3}),
      ('rdivI r1, 7 => r2',{'r1':2},{'r1':2,'r2':3}),
      ('lshiftI r1, 1 => r2',{'r1':8},{'r1':8,'r2':16}),
      ('rshiftI r1, 1 => r2',{'r1':8},{'r1':8,'r2':4}),
      ('andI r1, 1 => r2',{'r1':1},{'r1':1,'r2':1}),
      ('andI r1, 0 => r2',{'r1':1},{'r1':1,'r2':0}),
      ('orI r1, 1 => r2',{'r1':0},{'r1':0,'r2':1}),
      ('orI r1, 0 => r2',{'r1':0},{'r1':0,'r2':0}),
      ('xorI r1, 1 => r2',{'r1':1},{'r1':1,'r2':0}),
      ('xorI r1, 0 => r2',{'r1':1},{'r1':1,'r2':1}),
      ('load r1 => r1',{'r1':1},{'r1':3},{1:3},{1:3}),
      ('loadAI rbss, @x => r1',{},{'r1':3},{rbss:3},{rbss:3}),
      ('loadAI rbss, 1 => r1',{},{'r1':3},{(rbss+1):3},{(rbss+1):3}),
      ('loadAO rbss, r1 => r1',{'r1':0},{'r1':3},{rbss:3},{rbss:3}),
      ('store r1 => r2',{'r1':1,'r2':2},{'r1':1,'r2':2},{},{2:1}),
      ('storeAI r1 => rbss, @x',{'r1':3},{'r1':3},{},{rbss:3}),
      ('storeAO r1 => rbss, r1',{'r1':1},{'r1':1},{},{(rbss+1):1}),
      ('i2i r1 => r2',{'r1':1,'r2':2},{'r1':1,'r2':1}),
      ]
      for x in tests:
         self.test(*x)

def main():
   # Os argumentos são os seguintes
   parser = argparse.ArgumentParser(description='ILOC Simulator')
   # See https://stackoverflow.com/questions/8259001/python-argparse-command-line-flags-without-arguments
   parser.add_argument('-i', '--instructions', action='store_true')
   parser.add_argument('-m', '--memory', action='store_true')
   parser.add_argument('-s', '--stats', action='store_true')
   parser.add_argument('-t', '--trace', action='store_true')
   parser.add_argument('--data', type=int, default=0)
   parser.add_argument('--stack', type=int, default=1024)
   # If active (default), strict makes impossible
   # - any update of bss
   # - the first definition of stack (rfp/rsp)
   parser.add_argument('-x', '--strict', action='store_true')
   parser.add_argument('ILOCFILE', nargs='?')
   args = parser.parse_args()

   # Update the global strict
   global strict
   strict = args.strict

   # Ler do ILOCFILE, ou ler da entrada padrão (stdin)
   if args.ILOCFILE:
      with open(args.ILOCFILE) as f:
         program = f.read()
   else:
      program = sys.stdin.read()

   # Criar o simulador com o código do program (em program)
   sim = Sim(program, args.data, args.stack, args.trace)

   # Enquanto o tamanho do código do simulador for maior que o PC
   while len(sim.code) > sim.ip:
      # Realiza um passo de simulação
      sim.step()

   # Relata contagem de instruções executadas
   if (args.instructions):
     sim.show_instructions_executed_counting()

   # Relata estatísticas
   if (args.stats):
     sim.show_stats()

   # Relata o estado da memória
   if (args.memory):
     sim.show_memory()

if __name__ == '__main__': main()
