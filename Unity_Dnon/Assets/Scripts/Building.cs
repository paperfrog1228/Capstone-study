using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Building : MonoBehaviour
{
    [SerializeField] GameObject player;
    BoxCollider2D colider;
    float heieght, width;

    private void Start()
    {
        colider = GetComponent<BoxCollider2D>();
        heieght = colider.size.y * gameObject.transform.localScale.y;
        width = colider.size.x * gameObject.transform.localScale.x;
        Debug.Log(heieght + "  " + width);
    }
    bool CheckInPlayer()
    {// 건물 안에 플레이어가 있는지 체크함.
        Vector3 ppos = player.transform.position;
        Vector3 pos = transform.position;
        if (ppos.x < pos.x + width / 2 && pos.x - width / 2 < ppos.x &&
            ppos.y < pos.y + heieght / 2 && pos.y - heieght / 2 < ppos.y)
            return true;
        return false;
    }
    private void Update()//개인적으로 콜라이더로 바꾸고 싶음
    {
        if (CheckInPlayer())
            this.GetComponent<MeshRenderer>().material.SetColor("_Color", new Color(1f, 1f, 1f, 0.5f));
        else
            this.GetComponent<MeshRenderer>().material.SetColor("_Color", new Color(1f, 1f, 1f, 1f));
    }
}

