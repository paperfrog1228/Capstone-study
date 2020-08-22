using Sirenix.OdinInspector;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
// 미친놈이 기본 체력이랑 방어구 체력은 다르다고 이제 알려줌 다시 짠다.
// 체력 칸에는 반개가 존재합니까 -> 반 개 없음
// 체력 칸들은 가운데 정렬합니까 왼쪽 정렬합니까
// 체력 칸들이 쌓여 화면을 넘어가면 어떻게 처리합니까
// 캐릭터마다 시작 체력과 최대 체력이 다릅니까
public class HeartPanel : MonoBehaviour
{
    private int hp;
    public int Hp { get { return hp; } set { hp = value;onChangedHp(hp);}}
    [SerializeField] private List<Image> HpBar;
    [SerializeField] private Image hpSquare;
    [SerializeField] private Sprite fullHeart;
    [SerializeField] private Sprite emptyHeart;
    [SerializeField] private Sprite shield;
    //public Sprite HalfHeart; //반 개 없다했는데 나중에 생기면 죽일거다
    delegate void EventHandler(int hp);
    event EventHandler onChangedHp;
    [SerializeField]private int NumOfHpSquare=10;//하트 + 쉴드 칸
    [SerializeField]private int NumOfHeart=5;//하트칸. 그 이상은 쉴드로 받음.
    void Start()
    {   
        onChangedHp += UpdateNumberOfHeart;
        SetHpBar(NumOfHpSquare);
        Hp = 5;//나중에 const로 우겨넣자 
    }
    [Range(0,100),SerializeField] float interval = 50;
    private Vector3 lastHeartPos;
    [Button]
    private void SetHpBar(int n) 
    {
        lastHeartPos = HpBar[0].transform.position;
        for (int i = 1; i < n; i++)
        {
            HpBar.Add(Instantiate(hpSquare,this.transform));
            HpBar[i].transform.position = new Vector3(lastHeartPos.x + interval,lastHeartPos.y,lastHeartPos.z);
            lastHeartPos = HpBar[i].transform.position;
        }
    }
    [Button]
    void Test(int hp) {
        Hp = hp;
    }
    private void UpdateNumberOfHeart(int hp) {
        for (int i = 0; i < HpBar.Count; i++) {
            if (NumOfHpSquare<i) {
                HpBar[i].enabled = false;
                continue;
            }
            if (i < NumOfHeart)
            {
                if(i<Hp)
                HpBar[i].sprite = fullHeart;
                else
                HpBar[i].sprite = emptyHeart;
            }
            else
                HpBar[i].sprite = shield;
        }
    } 
}
