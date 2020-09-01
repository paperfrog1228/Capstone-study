using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class Mover : MonoBehaviour
{
    [SerializeField,Range(0,30)]
    private float speed=5.0f;

    void Update()
    {
        float horizontal = Input.GetAxis("Horizontal");
        float vertical = Input.GetAxis("Vertical");
        transform.Translate(Vector3.right * Time.deltaTime * speed * horizontal, Space.World);
        transform.Translate(Vector3.up  * Time.deltaTime * speed * vertical, Space.World);
    }
}
