const $ = (id) => document.getElementById(id);
let queue = [];
let historyStack = [];
let nextId = 101;
let currentView = "dashboard";
const priorityLabel = p => ({1:"Low",2:"Medium",3:"High"}[p] || "Low");
const badgeClass = p => ({1:"badge-low",2:"badge-medium",3:"badge-high"}[p]);
const esc = s => String(s).replace(/[&<>"']/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));
function addAction(text){ if(historyStack.length < 100) historyStack.push(text); }
function sortedInsert(req){
  // Stable priority queue: insert before strictly lower priority only.
  let i = queue.length;
  while(i>0 && queue[i-1].priority < req.priority){ i--; }
  queue.splice(i,0,req);
}
function toast(message){const t=$("toast");t.textContent=message;t.classList.add("show");clearTimeout(toast.timer);toast.timer=setTimeout(()=>t.classList.remove("show"),2600);}
function requestMarkup(r){return `<div class="request-row"><div class="request-id">#${r.id}</div><div class="request-main"><b>${esc(r.student)} · ${esc(r.category)}</b><small>${esc(r.description)}</small></div><span class="priority-badge ${badgeClass(r.priority)}">${priorityLabel(r.priority)}</span></div>`;}
function render(){
  $("pendingCount").textContent=queue.length;
  $("highCount").textContent=queue.filter(r=>r.priority===3).length;
  $("historyCount").textContent=historyStack.length;
  $("queueSub").textContent=`${queue.length} request${queue.length===1?"":"s"} waiting`;
  const q=queue.length?queue.map(requestMarkup).join(""):'<div class="empty">No pending requests yet. Submit your first request.</div>';
  $("dashboardQueue").className=queue.length?"queue-list":"queue-list empty"; $("dashboardQueue").innerHTML=q;
  $("fullQueue").className=queue.length?"queue-list":"queue-list empty"; $("fullQueue").innerHTML=queue.length?queue.map(requestMarkup).join(""):"No pending requests.";
  if(queue.length){const r=queue[0];$("nextPreview").innerHTML=`<div class="request-id">#${r.id}</div><b>${esc(r.student)}</b><small>${esc(r.category)} · ${priorityLabel(r.priority)} priority</small>`;}
  else $("nextPreview").innerHTML='<div class="empty-icon">↗</div><b>Queue is empty</b><small>Add a request to begin processing.</small>';
  const h=historyStack.length?historyStack.slice().reverse().map((a,i)=>`<div class="history-row"><span class="history-num">${i+1}</span><span>${esc(a)}</span></div>`).join(""):'<div class="empty">No actions recorded yet.</div>';
  $("historyList").className=historyStack.length?"history-list":"history-list empty";$("historyList").innerHTML=h;
}
function showView(view){
  currentView=view;
  document.querySelectorAll(".view").forEach(v=>v.classList.add("hidden"));
  $(`${view}View`).classList.remove("hidden");
  document.querySelectorAll(".nav").forEach(b=>b.classList.toggle("active",b.dataset.view===view));
  $("pageTitle").textContent=({dashboard:"Dashboard",requests:"Pending requests",history:"Action history"})[view];
  render();
}
function openModal(){$("modal").classList.remove("hidden");$("formError").textContent="";$("requestForm").reset();$("student").focus();}
function closeModal(){$("modal").classList.add("hidden");}
document.querySelectorAll(".nav").forEach(b=>b.addEventListener("click",()=>showView(b.dataset.view)));
document.querySelectorAll("[data-goto]").forEach(b=>b.addEventListener("click",()=>showView(b.dataset.goto)));
$("heroAdd").onclick=$("addRequestBtn").onclick=openModal;
$("closeModal").onclick=$("cancelModal").onclick=closeModal;
$("modal").addEventListener("click",e=>{if(e.target===$("modal"))closeModal();});
$("requestForm").addEventListener("submit",e=>{
  e.preventDefault();
  const student=$("student").value.trim(), category=$("category").value, description=$("description").value.trim();
  const p=Number(document.querySelector('input[name="priority"]:checked')?.value);
  if(!student||!category||!description||![1,2,3].includes(p)){$("formError").textContent="Please complete every field and select a priority.";return;}
  if(queue.length>=100){$("formError").textContent="Queue is full (maximum 100 pending requests).";return;}
  const r={id:nextId++,student,category,description,priority:p};
  sortedInsert(r);addAction(`Submitted request #${r.id} (${priorityLabel(p)} priority)`);
  closeModal();render();toast(`Request #${r.id} submitted successfully.`);showView(currentView);
});
function processNext(){
  if(!queue.length){toast("No pending requests to process.");return;}
  const r=queue.shift();addAction(`Processed request #${r.id} for ${r.student}`);
  render();toast(`Processed request #${r.id} · ${priorityLabel(r.priority)} priority`);
}
$("processBtn").onclick=processNext;
$("peekBtn").onclick=()=>{if(!historyStack.length)toast("Action history is empty.");else toast("Latest action: "+historyStack[historyStack.length-1]);};
$("popBtn").onclick=()=>{if(!historyStack.length){toast("Stack underflow: no action to remove.");return;}const a=historyStack.pop();render();toast("Popped: "+a);};
showView("dashboard");
